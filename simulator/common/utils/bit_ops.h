#ifndef SIMULATOR_COMMON_BIT_OPS_BIT_OPS_H
#define SIMULATOR_COMMON_BIT_OPS_BIT_OPS_H

#include "common/constants.h"
#include "common/config.h"

#include <cstddef>
#include <cstdint>
#include <cassert>

namespace rvsim {

template <typename T>
constexpr bit_size_t BitSizeof()
{
    return sizeof(T) * BITS_PER_BYTE;
}

constexpr dword_t Ones()
{
    return dword_t{0} - 1;
}

template <bit_size_t high, bit_size_t low>
constexpr dword_t GetBits(dword_t value)
{
    static_assert(high >= low);
    static_assert(high < BitSizeof<dword_t>());

    bit_size_t bit_size = high - low + 1;
    return (value >> low) & (Ones() >> (BitSizeof<dword_t>() - bit_size));
}

template <dword_t max, dword_t min>
constexpr dword_t Clamp(dword_t value)
{
    static_assert(max >= min);
    return value > max ? max : value < min ? min : value;
}

dword_t Ones   (bit_size_t high, bit_size_t low);
dword_t GetBits(bit_size_t high, bit_size_t low, dword_t value);

dword_t Clamp(dword_t max, dword_t min, dword_t value);

dword_t SignExtend(bit_size_t old_size, bit_size_t new_size, dword_t value);

} // namespace rvsim

#endif // SIMULATOR_COMMON_BIT_OPS_BIT_OPS_H
