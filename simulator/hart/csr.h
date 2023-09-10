#ifndef SIMULATOR_CSR_CSR_H
#define SIMULATOR_CSR_CSR_H

#include "constants.h"

#include <cstdint>

namespace rvsim {

static constexpr size_t N_CSR = 4096;
using csr_t = reg_t;
using csr_idx_t = uint16_t;

static constexpr csr_idx_t CSR_SATP_IDX = 0x180;

struct csr_satp {
    csr_t mode : 4;  // page virtual addressing mode
    csr_t asid : 16; // may be zero (unspecified)
    csr_t ppn  : 44; // physical page number (PPN) of the root page table
};

static_assert(sizeof(csr_satp) == sizeof(csr_t), "sizeof(csr_satp) != sizeof(csr)");

} // rvsim

#endif // SIMULATOR_CSR_CSR_H
