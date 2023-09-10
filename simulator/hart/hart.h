#ifndef SIMULATOR_HART_HART_H
#define SIMULATOR_HART_HART_H

#include "constants.h"
#include "csr.h"

#include <cstdint>

namespace rvsim {

class Hart {
private:
    csr_t csr_table_[N_CSR];

public:
    Hart() = default;
    ~Hart() = default;

    csr_t loadCSR(csr_idx_t index) const;
    void storeCSR(csr_idx_t index, csr_t reg);
};

} // rvsim

#endif // SIMULATOR_HART_HART_H
