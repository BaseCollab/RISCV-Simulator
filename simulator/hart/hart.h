#ifndef SIMULATOR_HART_HART_H
#define SIMULATOR_HART_HART_H

#include "common/constants.h"
#include "mmu/mmu.h"
#include "csr.h"

#include <cstdint>

namespace rvsim {

using reg_t = uint64_t;

class Hart {
private:
public:
    CSRs csr_regs;

    NO_COPY_SEMANTIC(Hart);
    NO_MOVE_SEMANTIC(Hart);

    Hart() = default;
    ~Hart() = default;
};

} // rvsim

#endif // SIMULATOR_HART_HART_H
