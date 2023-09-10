#ifndef SIMULATOR_CSR_CSR_H
#define SIMULATOR_CSR_CSR_H

#include "common/macros.h"
#include "common/constants.h"

#include <cstdint>

namespace rvsim {

static constexpr size_t N_CSR = 4096;
using csr_t = uint64_t;
using csr_idx_t = uint16_t;

class CSRs {
private:
    csr_t csr_table_[N_CSR];

public:
    CSRs()  = default;
    ~CSRs() = default;

    NO_COPY_SEMANTIC(CSRs);
    NO_MOVE_SEMANTIC(CSRs);

    csr_t LoadCSR(csr_idx_t index) const
    {
        return csr_table_[index];
    }

    void StoreCSR(csr_idx_t index, csr_t reg)
    {
        csr_table_[index] = reg;
    }
};

static constexpr csr_idx_t CSR_SATP_IDX = 0x180;

struct csr_satp {
    csr_t mode : 4;  // page virtual addressing mode
    csr_t asid : 16; // may be zero (unspecified)
    csr_t ppn  : 44; // physical page number (PPN) of the root page table
};

static_assert(sizeof(csr_satp) == sizeof(csr_t), "sizeof(csr_satp) != sizeof(csr)");

} // rvsim

#endif // SIMULATOR_CSR_CSR_H
