#ifndef SIMULATOR_HART_HART_H
#define SIMULATOR_HART_HART_H

#include "common/constants.h"
#include "common/macros.h"
#include "common/config.h"
#include "mmu/mmu.h"
#include "csr.h"

#include <cstdint>

namespace rvsim {

class Hart {
public:
    NO_COPY_SEMANTIC(Hart);
    NO_MOVE_SEMANTIC(Hart);

    explicit Hart(PhysMemoryCtl *memory) : memory_(memory) {};
    ~Hart() = default;

    const MMU &GetMMU() const
    {
        return mmu_;
    }

    reg_t GetPC() const;
    void SetPC(reg_t pc);

    reg_t GetPCTarget() const;
    void SetPCTarget(reg_t pc_target);

    gpr_t GetGPR(gpr_idx_t reg_idx) const;
    void SetGPR(gpr_idx_t reg_idx, gpr_t value);

public:
    CSRs csr_regs;

private:
    PhysMemoryCtl *memory_ {nullptr};
    MMU mmu_;

    gpr_t gpr_table_[N_GPR];

    reg_t pc_;
    reg_t pc_target_;
};

} // namespace rvsim

#endif // SIMULATOR_HART_HART_H
