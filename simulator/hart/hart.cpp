#include "hart.h"

#include <iostream>

namespace rvsim {

void Hart::LoadFromMemory(void *dst, size_t dst_size, vaddr_t src, uint8_t rwx_flags) const
{
    addr_t vpage_padding = (VPAGE_SIZE - src.value % VPAGE_SIZE) % VPAGE_SIZE;

    if (vpage_padding != 0) {
        auto pair_paddr = mmu_.VirtToPhysAddr(src, rwx_flags, csr_regs, *memory_);
        if (pair_paddr.second != MMU::Exception::NONE) {
            handlers_.mmu_handler(pair_paddr.second, src.value, rwx_flags);
            pair_paddr = mmu_.VirtToPhysAddr(src, rwx_flags, csr_regs, *memory_);
        }

        memory_->Load(dst, vpage_padding, pair_paddr.first.value);
    }

    for (addr_t vpage_offset = 0; vpage_offset < dst_size - vpage_padding; vpage_offset += VPAGE_SIZE) {
        auto pair_paddr = mmu_.VirtToPhysAddr(src.value + vpage_padding + vpage_offset, rwx_flags, csr_regs, *memory_);
        if (pair_paddr.second != MMU::Exception::NONE) {
            handlers_.mmu_handler(pair_paddr.second, src.value, rwx_flags);
            pair_paddr = mmu_.VirtToPhysAddr(src.value + vpage_padding + vpage_offset, rwx_flags, csr_regs, *memory_);
        }

        size_t load_size = VPAGE_SIZE;
        if ((dst_size - vpage_padding - vpage_offset) < VPAGE_SIZE)
            load_size = dst_size - vpage_padding - vpage_offset;

        memory_->Load((char *)dst + vpage_padding + vpage_offset, load_size, pair_paddr.first.value);
    }
}

void Hart::StoreToMemory(vaddr_t dst, void *src, size_t src_size, uint8_t rwx_flags) const
{
    addr_t vpage_padding = (VPAGE_SIZE - dst.value % VPAGE_SIZE) % VPAGE_SIZE;

    if (vpage_padding != 0) {
        auto pair_paddr = mmu_.VirtToPhysAddr(dst, rwx_flags, csr_regs, *memory_);
        if (pair_paddr.second != MMU::Exception::NONE) {
            handlers_.mmu_handler(pair_paddr.second, dst.value, rwx_flags);
            pair_paddr = mmu_.VirtToPhysAddr(dst, rwx_flags, csr_regs, *memory_);
        }

        memory_->Store(pair_paddr.first.value, src, vpage_padding);
    }

    for (addr_t vpage_offset = 0; vpage_offset < src_size - vpage_padding; vpage_offset += VPAGE_SIZE) {
        auto pair_paddr = mmu_.VirtToPhysAddr(dst.value + vpage_padding + vpage_offset, rwx_flags, csr_regs, *memory_);
        if (pair_paddr.second != MMU::Exception::NONE) {
            handlers_.mmu_handler(pair_paddr.second, dst.value, rwx_flags);
            pair_paddr = mmu_.VirtToPhysAddr(dst.value + vpage_padding + vpage_offset, rwx_flags, csr_regs, *memory_);
        }

        size_t store_size = VPAGE_SIZE;
        if ((src_size - vpage_padding - vpage_offset) < VPAGE_SIZE)
            store_size = src_size - vpage_padding - vpage_offset;

        memory_->Store(pair_paddr.first.value, (char *)src + vpage_padding + vpage_offset, store_size);
    }
}

instr_size_t Hart::FetchInstruction()
{
    instr_size_t raw_instr = LoadFromMemory<instr_size_t>(vaddr_t(pc_), 0 | PF_R | PF_X);
    return raw_instr;
}

void Hart::Interpret()
{
    // start interpreting instructions
    is_idle_ = false;

    while (!is_idle_) {
        instr_size_t raw_instr = FetchInstruction();

        Instruction instr;
        DecodeAndExecute(&instr, raw_instr);

        pc_ = pc_target_;
    }
}

} // namespace rvsim
