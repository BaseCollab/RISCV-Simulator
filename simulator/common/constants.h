#ifndef SIMULATOR_COMMON_CONSTANTS_H
#define SIMULATOR_COMMON_CONSTANTS_H

#include <cstddef>
#include <cstdint>

namespace rvsim {

static constexpr size_t HWORD_SZ = 2;
static constexpr size_t WORD_SZ  = 4;
static constexpr size_t DWORD_SZ = 8;

static constexpr size_t BITS_PER_BYTE = 8;
static constexpr size_t KBYTE_SZ = (1 << 10);
static constexpr size_t MBYTE_SZ = (1 << 20);

} // namespace rvsim

#endif // SIMULATOR_COMMON_CONSTANTS_H