#include "hart.h"

#include <cassert>
#include <iostream>

namespace rvsim {

void Hart::SetExceptionHandlers(const ExceptionHandlers &handlers)
{
    handlers_ = handlers;
}

reg_t Hart::GetPC() const
{
    return pc_;
}

reg_t Hart::GetPCTarget() const
{
    return pc_target_;
}

void Hart::SetPC(reg_t pc)
{
    pc_ = pc;
}

void Hart::SetPCTarget(reg_t pc_target)
{
    pc_target_ = pc_target;
}

gpr_t Hart::GetGPR(gpr_idx_t reg_idx) const
{
    assert(reg_idx < N_GPR);
    return gpr_table_[reg_idx];
}

void Hart::SetGPR(gpr_idx_t reg_idx, gpr_t value)
{
    assert(reg_idx < N_GPR);
    gpr_table_[reg_idx] = value;
}

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

    for (addr_t vpage_offset = 0; vpage_offset < dst_size - vpage_padding; vpage_offset += VPAGE_SIZE)
    {
        auto pair_paddr = mmu_.VirtToPhysAddr(src.value + vpage_padding + vpage_offset, rwx_flags, csr_regs, *memory_);
        if (pair_paddr.second != MMU::Exception::NONE) {
            handlers_.mmu_handler(pair_paddr.second, src.value, rwx_flags);
            pair_paddr = mmu_.VirtToPhysAddr(src.value + vpage_padding + vpage_offset, rwx_flags, csr_regs, *memory_);
        }

        size_t load_size = VPAGE_SIZE;
        if ((dst_size - vpage_padding - vpage_offset) < VPAGE_SIZE)
            load_size = dst_size - vpage_padding - vpage_offset;

        memory_->Load((char *) dst + vpage_padding + vpage_offset, load_size, pair_paddr.first.value);
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

    for (addr_t vpage_offset = 0; vpage_offset < src_size - vpage_padding; vpage_offset += VPAGE_SIZE)
    {
        auto pair_paddr = mmu_.VirtToPhysAddr(dst.value + vpage_padding + vpage_offset, rwx_flags, csr_regs, *memory_);
        if (pair_paddr.second != MMU::Exception::NONE) {
            handlers_.mmu_handler(pair_paddr.second, dst.value, rwx_flags);
            pair_paddr = mmu_.VirtToPhysAddr(dst.value + vpage_padding + vpage_offset, rwx_flags, csr_regs, *memory_);
        }

        size_t store_size = VPAGE_SIZE;
        if ((src_size - vpage_padding - vpage_offset) < VPAGE_SIZE)
            store_size = src_size - vpage_padding - vpage_offset;

        memory_->Store(pair_paddr.first.value, (char *) src + vpage_padding + vpage_offset, store_size);
    }
}

template <typename ValueType>
ValueType Hart::LoadFromMemory(vaddr_t src, uint8_t rwx_flags) const
{
    auto pair_paddr = mmu_.VirtToPhysAddr(src, rwx_flags, csr_regs, *memory_);
    if (pair_paddr.second != MMU::Exception::NONE) {
        handlers_.mmu_handler(pair_paddr.second, GetPC(), rwx_flags);
        pair_paddr = mmu_.VirtToPhysAddr(src, rwx_flags, csr_regs, *memory_);
    }

    auto load_pair = memory_->Load<ValueType>(pair_paddr.first.value);
    return load_pair.second;
}

template <typename ValueType>
void Hart::StoreToMemory(vaddr_t dst, ValueType value, uint8_t rwx_flags) const
{
    auto pair_paddr = mmu_.VirtToPhysAddr(dst, rwx_flags, csr_regs, *memory_);
    if (pair_paddr.second != MMU::Exception::NONE) {
        handlers_.mmu_handler(pair_paddr.second, GetPC(), rwx_flags);
        pair_paddr = mmu_.VirtToPhysAddr(dst, rwx_flags, csr_regs, *memory_);
    }

    memory_->Store<ValueType>(pair_paddr.first.value, value);
}

} // namespace rvsim
