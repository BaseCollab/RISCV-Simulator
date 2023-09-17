#include "common/utils/bit_ops.h"

#include <cstddef>
#include <cstdint>
#include <cassert>

namespace rvsim {

dword_t Ones(bit_size_t high, bit_size_t low)
{
    return GetBits(high, low, Ones()) << low;
}

dword_t GetBits(bit_size_t high, bit_size_t low, dword_t value)
{
    assert(high >= low);
    assert(high < BitSizeof<dword_t>());

    bit_size_t bit_size = high - low + 1;
    return (value >> low) & (Ones() >> (BitSizeof<dword_t>() - bit_size));
}

dword_t Clamp(dword_t max, dword_t min, dword_t value)
{
    assert(max >= min);
    return value > max ? max : value < min ? min : value;
}

dword_t SignExtend(bit_size_t old_size, bit_size_t new_size, dword_t value)
{
    bit_size_t shift = Clamp<BitSizeof<dword_t>(), 0>(new_size - old_size);
    dword_t sign = GetBits(old_size - 1, old_size - 1, value);
    dword_t mask = ((dword_t{1} << shift) - sign) << old_size;
    return GetBits(new_size - 1, 0, mask | value);
}

} // namespace rvsim
