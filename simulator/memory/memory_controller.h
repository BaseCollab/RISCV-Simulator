#ifndef SIMULATOR_MEMORY_MEMORY_CONTROLLER_H
#define SIMULATOR_MEMORY_MEMORY_CONTROLLER_H

#include "common/constants.h"
#include "memory.h"
#include "pages_controller.h"

#include <cstddef>
#include <cstdint>
#include <optional>

namespace rvsim {

class MemoryCtl {
public:
    enum class Error {
        NONE = -1,
        STORE_BOUNDARY_CHK = 0,
        LOAD_BOUNDARY_CHK = 1
    };

    using PageError = PagesController::Error;

public:
    NO_COPY_SEMANTIC(MemoryCtl);
    NO_MOVE_SEMANTIC(MemoryCtl);

    MemoryCtl() = default;
    ~MemoryCtl() = default;

    template <typename ValueType>
    std::optional<Error> Store(addr_t dst, ValueType value)
    {
        if (!BoundaryCheck(dst, sizeof(ValueType))) {
            return Error::STORE_BOUNDARY_CHK;
        }

        memory_.Store<ValueType>(dst, value);
    }

    template <typename ValueType>
    std::pair<ValueType, std::optional<Error>> Load(addr_t src) const
    {
        if (!BoundaryCheck(src, sizeof(ValueType))) {
            return Error::LOAD_BOUNDARY_CHK;
        }

        memory_.Load<ValueType>(src);
    }

    // Store src_size bytes from src to dst in ram_
    std::optional<Error> Store(addr_t dst, void *src, size_t src_size)
    {
        if (!BoundaryCheck(dst, src_size)) {
            return Error::STORE_BOUNDARY_CHK;
        }

        memory_.Store(dst, src, src_size);

        return std::nullopt;
    }

    std::optional<Error> Load(void *dst, size_t dst_size, addr_t src) const
    {
        if (!BoundaryCheck(src, dst_size)) {
            return Error::LOAD_BOUNDARY_CHK;
        }

        memory_.Load(dst, dst_size, src);

        return std::nullopt;
    }

    std::pair<uint16_t, std::optional<PageError>> GetCleanPage()
    {
        return pages_controller_.GetCleanPage();
    }

private:
    bool BoundaryCheck(addr_t dst, size_t value_size) const
    {
        return (dst + value_size < memory_.GetMemorySize());
    }

private:
    Memory memory_;
    PagesController pages_controller_;
};

} // namespace rvsim

#endif // SIMULATOR_MEMORY_MEMORY_CONTROLLER_H
