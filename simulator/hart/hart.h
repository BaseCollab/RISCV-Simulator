#ifndef SIMULATOR_HART_HART_H
#define SIMULATOR_HART_HART_H

#include "constants.h"
#include "csr.h"
#include "mmu.h"

#include <cstdint>

namespace rvsim {

using reg_t = uint64_t;

class Hart {
private:
    csr_t csr_table_[N_CSR];

public:
    NO_COPY_SEMANTIC(Hart);
    NO_MOVE_SEMANTIC(Hart);

    Hart() = default;
    ~Hart() = default;

    csr_t LoadCSR(csr_idx_t index) const;
    void StoreCSR(csr_idx_t index, csr_t reg);
};

} // rvsim

#endif // SIMULATOR_HART_HART_H
