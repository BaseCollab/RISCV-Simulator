#ifndef SIMULATOR_COMMON_BIT_OPS_STACK_H
#define SIMULATOR_COMMON_BIT_OPS_STACK_H

#include <cstddef>
#include <cstdint>

template <uint8_t high, uint8_t low, typename T>
T getBits(T value)
{
    T mask = ((1UL << (high - low + 1)) - 1) << low;
    return (value & mask);
}

#endif // SIMULATOR_COMMON_BIT_OPS_STACK_H
