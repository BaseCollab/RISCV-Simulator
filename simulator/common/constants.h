#ifndef SIMULATOR_COMMON_CONSTANTS_H
#define SIMULATOR_COMMON_CONSTANTS_H

#include <cstddef>
#include <cstdint>

namespace rvsim {

static constexpr size_t HWORD_SIZE = 2;
static constexpr size_t WORD_SIZE = 4;
static constexpr size_t DWORD_SIZE = 8;

static constexpr size_t BITS_PER_BYTE = 8;
static constexpr size_t KBYTE_SIZE = (1 << 10);
static constexpr size_t MBYTE_SIZE = (1 << 20);

static constexpr size_t VPAGE_SIZE = 4 * KBYTE_SIZE;
static constexpr size_t PPAGE_SIZE = VPAGE_SIZE;

using addr_t = uint64_t;

} // namespace rvsim

#endif // SIMULATOR_COMMON_CONSTANTS_H
