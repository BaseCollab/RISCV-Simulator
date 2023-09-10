#ifndef SIMULATOR_COMMON_MACROS_H
#define SIMULATOR_COMMON_MACROS_H

#define NO_COPY_CTOR(TypeName) TypeName(const TypeName &) = delete
#define NO_COPY_OPERATOR(TypeName) TypeName &operator=(const TypeName &) = delete

#define NO_MOVE_CTOR(TypeName) TypeName(TypeName &&) = delete
#define NO_MOVE_OPERATOR(TypeName) TypeName &operator=(TypeName &&) = delete

#define NO_COPY_SEMANTIC(TypeName) \
    NO_COPY_CTOR(TypeName);        \
    NO_COPY_OPERATOR(TypeName)

#define NO_MOVE_SEMANTIC(TypeName) \
    NO_MOVE_CTOR(TypeName);        \
    NO_MOVE_OPERATOR(TypeName)

template<uint8_t high, uint8_t low, typename T>
T getBits(T value)
{
    T mask = ((1UL << (high - low + 1)) - 1) << low;
    return (value & mask);
}

#endif // SIMULATOR_COMMON_MACROS_H
