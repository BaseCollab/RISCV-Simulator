#ifndef SIMULATOR_COMMON_CONFIG_H
#define SIMULATOR_COMMON_CONFIG_H

#include "constants.h"

#include <cstddef>
#include <cstdint>

namespace rvsim {

// clang-format off

static constexpr size_t VPAGE_SIZE = 4 * KBYTE_SIZE;
static constexpr size_t PPAGE_SIZE = VPAGE_SIZE;

using addr_t     = dword_t;
using bit_size_t = dword_t;

enum class Mode : uint8_t {
    USER_MODE       = 0,
    SUPERVISOR_MODE = 1,
    // RESERVED        = 2,
    MACHINE_MODE    = 3
};

// clang-format on

} // namespace rvsim

#endif // SIMULATOR_COMMON_CONFIG_H
