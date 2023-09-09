#ifndef SIMULATOR_MEMORY_MEMORY_H
#define SIMULATOR_MEMORY_MEMORY_H

#include "macros.h"
#include "constants.h"

#include <cstdint>
#include <cstring>
#include <optional>
#include <utility>

namespace rvsim {

// Class that simulate physical memory
class Memory {
public:
    enum class Error
    {
        NONE = -1
    };

public:
    NO_COPY_SEMANTIC(Memory);
    NO_MOVE_SEMANTIC(Memory);

    Memory() = default;
    ~Memory() = default;

    template<typename ValueType>
    Error Store(paddr addr, ValueType value)
    {
        memcpy(ram_ + addr, &value, sizeof(value));
        return Error::NONE;
    }

    template<typename ValueType>
    std::pair<ValueType, Error> Load(paddr addr)
    {
        ValueType value = 0;
        memcpy(&value, ram_ + addr, sizeof(value));
        return std::make_pair<value, Error::NONE>;
    }

private:
    static constexpr size_t MEMORY_SZ = 32 * MBYTE_SZ; // 1 << 25

    uint8_t ram_[MEMORY_SZ];
};

} // rvsim

#endif // SIMULATOR_MEMORY_MEMORY_H
