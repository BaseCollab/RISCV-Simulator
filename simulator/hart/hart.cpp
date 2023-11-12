#include "hart.h"

#include <iostream>

namespace rvsim {

Exception Hart::LoadFromMemory(void *dst, size_t dst_size, vaddr_t src, uint8_t rwx_flags) const
{
    addr_t vpage_padding = (VPAGE_SIZE - src % VPAGE_SIZE) % VPAGE_SIZE;

    paddr_t paddr {0};
    Exception exception {Exception::NONE};

    if (vpage_padding != 0) {
        std::tie(paddr, exception) = mmu_.VirtToPhysAddr(src, rwx_flags, csr_regs, *memory_);
        if (exception != Exception::NONE) {
            return exception;
        }

        memory_->Load(dst, vpage_padding, paddr);
    }

    for (addr_t vpage_offset = 0; vpage_offset < dst_size - vpage_padding; vpage_offset += VPAGE_SIZE) {
        std::tie(paddr, exception) = mmu_.VirtToPhysAddr(src + vpage_padding + vpage_offset, rwx_flags, csr_regs, *memory_);
        if (exception != Exception::NONE) {
            return exception;
        }

        size_t load_size = VPAGE_SIZE;
        if ((dst_size - vpage_padding - vpage_offset) < VPAGE_SIZE)
            load_size = dst_size - vpage_padding - vpage_offset;

        memory_->Load((char *)dst + vpage_padding + vpage_offset, load_size, paddr);
    }

    return Exception::NONE;
}

Exception Hart::StoreToMemory(vaddr_t dst, void *src, size_t src_size, uint8_t rwx_flags) const
{
    addr_t vpage_padding = (VPAGE_SIZE - dst % VPAGE_SIZE) % VPAGE_SIZE;

    paddr_t paddr {0};
    Exception exception {Exception::NONE};

    if (vpage_padding != 0) {
        std::tie(paddr, exception) = mmu_.VirtToPhysAddr(dst, rwx_flags, csr_regs, *memory_);
        if (exception != Exception::NONE) {
            return exception;
        }

        memory_->Store(paddr, src, vpage_padding);
    }

    for (addr_t vpage_offset = 0; vpage_offset < src_size - vpage_padding; vpage_offset += VPAGE_SIZE) {
        std::tie(paddr, exception) = mmu_.VirtToPhysAddr(dst + vpage_padding + vpage_offset, rwx_flags, csr_regs, *memory_);
        if (exception != Exception::NONE) {
            return exception;
        }

        size_t store_size = VPAGE_SIZE;
        if ((src_size - vpage_padding - vpage_offset) < VPAGE_SIZE)
            store_size = src_size - vpage_padding - vpage_offset;

        memory_->Store(paddr, (char *)src + vpage_padding + vpage_offset, store_size);
    }

    return Exception::NONE;
}

Exception Hart::FetchInstruction(instr_size_t *raw_instr) const
{
#ifdef DEBUG_HART
#ifdef DEBUG
    std::cerr << "[DEBUG] ============================================="
                 "============================================="
              << std::endl;
#endif
    std::cerr << "[DEBUG] [FETCH] PC = 0x" << std::hex << pc_ << std::dec << std::endl;
#endif

    return LoadFromMemory<instr_size_t>(vaddr_t(pc_), raw_instr, PF_R | PF_X);
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
