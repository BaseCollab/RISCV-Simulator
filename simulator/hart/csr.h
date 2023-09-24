#ifndef SIMULATOR_CSR_CSR_H
#define SIMULATOR_CSR_CSR_H

#include "common/macros.h"
#include "common/constants.h"
#include "common/config.h"

#include <cstdint>

namespace rvsim {

// Control and Status Register
class CSRs {
private:
    csr_t csr_table_[N_CSR];

public:
    NO_COPY_SEMANTIC(CSRs);
    NO_MOVE_SEMANTIC(CSRs);

    CSRs() = default;
    ~CSRs() = default;

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

// clang-format off
union csr_satp_t {
    struct CsrFileds {
        csr_t mode : 4  = 0; // page virtual addressing mode
        csr_t asid : 16 = 0; // may be zero (unspecified)
        csr_t ppn  : 44 = 0; // physical page number (PPN) of the root page table
    };

    csr_satp_t(csr_t csr_val): csr(csr_val) {} 
    csr_satp_t(CsrFileds fld)
    {
        fields.mode = fld.mode;
        fields.asid = fld.asid;
        fields.ppn  = fld.ppn;
    }

    CsrFileds fields;
    csr_t csr;
};
// clang-format on

static_assert(sizeof(csr_satp_t) == sizeof(csr_t), "sizeof(csr_satp_t) != sizeof(csr_t)");

} // namespace rvsim

#endif // SIMULATOR_CSR_CSR_H
