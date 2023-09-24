#include "hart.h"

#include <assert>

namespace rvsim {

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

} // namespace rvsim
