#ifndef SIMULATOR_MEMORY_MEMORY_H
#define SIMULATOR_MEMORY_MEMORY_H

#include "macros.h"
#include "constants.h"
#include "vpt.h"

#include <cstdint>
#include <cstring>
#include <optional>
#include <utility>
#include "utils/stack.h"

namespace rvsim {

// Class that simulate physical memory
class Memory {
public:
    NO_COPY_SEMANTIC(Memory);
    NO_MOVE_SEMANTIC(Memory);

    Memory()  = default;
    ~Memory() = default;

    // Store ValueType value in dst
    template<typename ValueType>
    void Store(paddr_t dst, ValueType value)
    {
        std::memcpy(ram_ + dst, &value, sizeof(value));
    }

    // Load ValueType value from src
    template<typename ValueType>
    ValueType Load(paddr_t src) const
    {
        ValueType value = 0;
        std::memcpy(&value, ram_ + src, sizeof(value));
        return value;
    }

    // Store src_size bytes from src to dst in ram_
    void Store(paddr_t dst, void *src, size_t src_size)
    {
        std::memcpy(ram_ + dst, src, src_size);
    }

    // Load dst_size bytes from src ram_ to dst
    void Load(void *dst, size_t dst_size, paddr_t src) const
    {
        std::memcpy(dst, ram_ + src, dst_size);
    }

    static constexpr size_t GetMemorySize()
    {
        return MEMORY_SIZE;
    }

private:
    static constexpr size_t MEMORY_SIZE = 32 * MBYTE_SIZE; // 1 << 25

    uint8_t ram_[MEMORY_SIZE];
};

} // rvsim

#endif // SIMULATOR_MEMORY_MEMORY_H
