#ifndef SIMULATOR_MEMORY_MEMORY_CONTROLLER_H
#define SIMULATOR_MEMORY_MEMORY_CONTROLLER_H

#include "memory.h"
#include <cstddef>
#include <cstdint>

namespace rvsim {

class MemoryCtl {
public:
    NO_COPY_SEMANTIC(MemoryCtl);
    NO_MOVE_SEMANTIC(MemoryCtl);

    MemoryCtl() = default;
    ~MemoryCtl() = default;

    void Store();
    void Load();

private:
    // TODO(R.Glaz): move PAGE_SIZE to hart
    static constexpr size_t PAGE_SIZE = 8 * 1024;

private:
    Memory memory_;

};

} // namespace rvsim

#endif // SIMULATOR_MEMORY_MEMORY_CONTROLLER_H
