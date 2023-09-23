#ifndef SIMULATOR_HART_HART_H
#define SIMULATOR_HART_HART_H

#include "common/constants.h"
#include "mmu/mmu.h"
#include "csr.h"

#include <cstdint>

namespace rvsim {

class Hart {
public:
    NO_COPY_SEMANTIC(Hart);
    NO_MOVE_SEMANTIC(Hart);

    explicit Hart(MemoryCtl *memory) : memory_(memory) {};
    ~Hart() = default;

    const MMU &GetMMU() const
    {
        return mmu_;
    }

public:
    CSRs csr_regs;

private:
    MemoryCtl *memory_ {nullptr};
    MMU mmu_;
};

} // namespace rvsim

#endif // SIMULATOR_HART_HART_H
