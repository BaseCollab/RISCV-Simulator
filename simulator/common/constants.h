#ifndef SIMULATOR_COMMON_CONSTANTS_H
#define SIMULATOR_COMMON_CONSTANTS_H

#include <cstddef>
#include <cstdint>

namespace rvsim {

using byte_t = uint8_t;
static constexpr size_t BYTE_SIZE = sizeof(byte_t);

using hword_t = uint16_t;
static constexpr size_t HWORD_SIZE = sizeof(hword_t);

using word_t = uint32_t;
static constexpr size_t WORD_SIZE = sizeof(word_t);

using dword_t = uint64_t;
static constexpr size_t DWORD_SIZE = sizeof(dword_t);

static constexpr size_t BITS_PER_BYTE = 8;
static constexpr size_t KBYTE_SIZE = (1 << 10);
static constexpr size_t MBYTE_SIZE = (1 << 20);

static constexpr size_t VPAGE_SIZE = 4 * KBYTE_SIZE;
static constexpr size_t PPAGE_SIZE = VPAGE_SIZE;

using addr_t = dword_t;

} // namespace rvsim

#endif // SIMULATOR_COMMON_CONSTANTS_H
