#ifndef SIMULATOR_COMMON_CONFIG_H
#define SIMULATOR_COMMON_CONFIG_H

#include "constants.h"

#include <cstddef>
#include <cstdint>

namespace rvsim {

// clang-format off

static constexpr size_t VPAGE_SIZE = 4 * KBYTE_SIZE;
static constexpr size_t PPAGE_SIZE = VPAGE_SIZE;

using instr_size_t = word_t;

using reg_t      = dword_t;
using addr_t     = dword_t;
using bit_size_t = dword_t;

using gpr_t     = reg_t; // GPR = General Purpose Register
using gpr_idx_t = byte_t;
static constexpr size_t N_GPR = 1 << 5;

using csr_t     = reg_t; // CSR = Control System Register
using csr_idx_t = hword_t;
static constexpr size_t N_CSR = 1 << 12;

enum class Mode : byte_t {
    USER_MODE       = 0,
    SUPERVISOR_MODE = 1,
    HYPERVISOR_MODE = 2,
    MACHINE_MODE    = 3
};

// clang-format on

} // namespace rvsim

#endif // SIMULATOR_COMMON_CONFIG_H
