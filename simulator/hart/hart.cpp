#include "hart.h"

#include <iostream>

namespace rvsim {

Exception Hart::LoadFromMemory(void *dst, size_t dst_size, vaddr_t src, uint8_t rwx_flags)
{
    addr_t vpage_padding = (VPAGE_SIZE - src % VPAGE_SIZE) % VPAGE_SIZE;

    paddr_t paddr {0};
    Exception exception {Exception::NONE};

    if (vpage_padding != 0) {
        vaddr_t src_page_addr = src & ~vaddr_t::mask_page_offset;
        const TLB_t::Data *cached_addr = rtlb_.LookUp(src_page_addr);

        if (cached_addr == nullptr) {
            std::tie(paddr, exception) = mmu_.VirtToPhysAddr(src, rwx_flags, csr_regs, *memory_);
            if (exception != Exception::NONE) {
                return exception;
            }

            rtlb_.Update(TLB_t::Data {.host_addr = nullptr, .paddr = (paddr & ~paddr_t::mask_page_offset)}, src_page_addr);
        } else {
            paddr = cached_addr->paddr + (src & vaddr_t::mask_page_offset);
        }

        memory_->Load(dst, vpage_padding, paddr);
    }

    for (addr_t vpage_offset = 0; vpage_offset < dst_size - vpage_padding; vpage_offset += VPAGE_SIZE) {
        vaddr_t page_addr = (src + vpage_padding + vpage_offset) & ~vaddr_t::mask_page_offset;
        const TLB_t::Data *cached_addr = rtlb_.LookUp(page_addr);

        if (cached_addr == nullptr) {
            std::tie(paddr, exception) = mmu_.VirtToPhysAddr(page_addr, rwx_flags, csr_regs, *memory_);
            if (exception != Exception::NONE) {
                return exception;
            }

            rtlb_.Update(TLB_t::Data {.host_addr = nullptr, .paddr = paddr}, page_addr);
        } else {
            paddr = cached_addr->paddr;
        }

        size_t load_size = VPAGE_SIZE;
        if ((dst_size - vpage_padding - vpage_offset) < VPAGE_SIZE)
            load_size = dst_size - vpage_padding - vpage_offset;

        memory_->Load((char *)dst + vpage_padding + vpage_offset, load_size, paddr);
    }

    return Exception::NONE;
}

Exception Hart::StoreToMemory(vaddr_t dst, void *src, size_t src_size, uint8_t rwx_flags)
{
    addr_t vpage_padding = (VPAGE_SIZE - dst % VPAGE_SIZE) % VPAGE_SIZE;

    paddr_t paddr {0};
    Exception exception {Exception::NONE};

    if (vpage_padding != 0) {
        vaddr_t dst_page_addr = dst & ~vaddr_t::mask_page_offset;
        const TLB_t::Data *cached_addr = wtlb_.LookUp(dst_page_addr);

        if (cached_addr == nullptr) {
            std::tie(paddr, exception) = mmu_.VirtToPhysAddr(dst, rwx_flags, csr_regs, *memory_);
            if (exception != Exception::NONE) {
                return exception;
            }

            wtlb_.Update(TLB_t::Data {.host_addr = nullptr, .paddr = (paddr & ~paddr_t::mask_page_offset)}, dst_page_addr);
        } else {
            paddr = cached_addr->paddr + (dst & vaddr_t::mask_page_offset);
        }

        memory_->Store(paddr, src, vpage_padding);
    }

    for (addr_t vpage_offset = 0; vpage_offset < src_size - vpage_padding; vpage_offset += VPAGE_SIZE) {
        vaddr_t page_addr = (dst + vpage_padding + vpage_offset) & ~vaddr_t::mask_page_offset;
        const TLB_t::Data *cached_addr = wtlb_.LookUp(page_addr);

        if (cached_addr == nullptr) {
            std::tie(paddr, exception) = mmu_.VirtToPhysAddr(page_addr, rwx_flags, csr_regs, *memory_);
            if (exception != Exception::NONE) {
                return exception;
            }

            wtlb_.Update(TLB_t::Data {.host_addr = nullptr, .paddr = paddr}, page_addr);
        } else {
            paddr = cached_addr->paddr;
        }

        size_t store_size = VPAGE_SIZE;
        if ((src_size - vpage_padding - vpage_offset) < VPAGE_SIZE)
            store_size = src_size - vpage_padding - vpage_offset;

        memory_->Store(paddr, (char *)src + vpage_padding + vpage_offset, store_size);
    }

    return Exception::NONE;
}

// TODO: create own fetch exceptions
Exception Hart::FetchInstruction(instr_size_t *raw_instr)
{
#ifdef DEBUG_HART
#ifdef DEBUG
    std::cerr << "[DEBUG] ============================================="
                 "============================================="
              << std::endl;
#endif
    std::cerr << "[DEBUG] [FETCH] PC = 0x" << std::hex << pc_ << std::dec << std::endl;
#endif

    vaddr_t pc = vaddr_t(pc_);

    if ((pc & (sizeof(instr_size_t) - 1)) != 0) {
        return Exception::MMU_ADDRESS_MISALIGNED;
    }

    byte_t *host_addr {nullptr};
    Exception exception {Exception::NONE};

    vaddr_t pc_page_addr = pc & ~vaddr_t::mask_page_offset;

    const TLB_t::Data *cached_addr = itlb_.LookUp(pc_page_addr);
    if (cached_addr == nullptr) {
        paddr_t paddr {0};
        std::tie(paddr, exception) = mmu_.VirtToPhysAddr(pc, PF_R | PF_X, csr_regs, *memory_);
        if (exception != Exception::NONE) {
            return exception;
        }

        paddr_t tlb_paddr = (paddr & ~paddr_t::mask_page_offset);
        itlb_.Update(TLB_t::Data {.host_addr = (memory_->GetRAM() + tlb_paddr), .paddr = tlb_paddr}, pc_page_addr);

        host_addr = memory_->GetRAM() + tlb_paddr + (pc & vaddr_t::mask_page_offset);
    } else {
        host_addr = cached_addr->host_addr + (pc & vaddr_t::mask_page_offset);
    }

    memcpy(raw_instr, host_addr, sizeof(instr_size_t));

    return exception;
}

void Hart::Interpret()
{
    // start interpreting instructions
    is_idle_ = false;

    while (true) {
        instr_size_t raw_instr {0};
        Instruction instr;
        Exception exception = Exception::NONE;

        exception = FetchInstruction(&raw_instr);
        if (exception != Exception::NONE) {
            handlers_.mmu_handler(exception, vaddr_t(pc_));
        }

        // TODO: rewrite exception handling here
        exception = DecodeAndExecute(&instr, raw_instr);
        if (exception != Exception::NONE) {
            handlers_.mmu_handler(exception, vaddr_t(pc_));
        }

#ifdef DEBUG_HART
        DumpRegs(std::cerr);
#endif

        pc_ = pc_target_;

        if (pc_ == 0 || is_idle_) {
            break;
        }
    }
}

} // namespace rvsim
