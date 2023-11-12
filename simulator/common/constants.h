#ifndef SIMULATOR_COMMON_CONSTANTS_H
#define SIMULATOR_COMMON_CONSTANTS_H

#include <cstddef>
#include <cstdint>

namespace rvsim {

static constexpr uint8_t PF_X = (1 << 0); // Executable flag
static constexpr uint8_t PF_W = (1 << 1); // Writable flag
static constexpr uint8_t PF_R = (1 << 2); // Readable flag
static constexpr uint8_t PF_U = (1 << 3); // User-mode flag

// clang-format off

using byte_t = uint8_t;
static constexpr size_t BYTE_SIZE = sizeof(byte_t);

using hword_t = uint16_t;
static constexpr size_t HWORD_SIZE = sizeof(hword_t);

using word_t = uint32_t;
static constexpr size_t WORD_SIZE = sizeof(word_t);

using dword_t = uint64_t;
static constexpr size_t DWORD_SIZE = sizeof(dword_t);

static constexpr size_t BITS_PER_BYTE = 1 << 3;
static constexpr size_t KBYTE_SIZE    = 1 << 10;
static constexpr size_t MBYTE_SIZE    = 1 << 20;

// clang-format on

} // namespace rvsim

#endif // SIMULATOR_COMMON_CONSTANTS_H
