#include "plugin.h"
#include "plugin_handler.h"

namespace rvsim {
rvsim::reg_t Plugin::GetPC() const
{
    return handler_->GetPC();
}

rvsim::reg_t Plugin::GetGPR(gpr_idx_t reg_idx) const
{
    return handler_->GetGPR(reg_idx);
}

std::vector<rvsim::reg_t> Plugin::GetRegFile() const
{
    return handler_->GetRegFile();
}
} // namespace rvsim