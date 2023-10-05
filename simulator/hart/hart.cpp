#include "hart.h"

#include <cassert>

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
    (void)dst;
    (void)dst_size;
    (void)src;
    (void)rwx_flags;
}

void Hart::StoreToMemory(vaddr_t dst, void *src, size_t src_size, uint8_t rwx_flags) const
{
    (void)dst;
    (void)src_size;
    (void)src;
    (void)rwx_flags;
}

template <typename ValueType>
reg_t Hart::LoadFromMemory(vaddr_t src, uint8_t rwx_flags) const
{
    // TODO: Page-fault handling
    auto pair = memory_->Load<ValueType>(src);
    return pair.second;
}

template <typename ValueType>
void Hart::StoreToMemory(vaddr_t dst, reg_t value, uint8_t rwx_flags) const
{
    // TODO: Page-fault handling
    memory_->Store<ValueType>(dst, value);
}

} // namespace rvsim
