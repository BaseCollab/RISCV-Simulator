#ifndef SIMULATOR_COMMON_UTILS_STACK_H
#define SIMULATOR_COMMON_UTILS_STACK_H

#include "common/macros.h"

#include <cstddef>
#include <optional>
#include <utility>

namespace rvsim::utils {

template <typename Type, size_t capacity>
class Stack {
public:
    enum class Error {
        NONE = -1,
        OVERFLOW = 0,
        EMPTY_POP = 1,
        EMPTY_TOP
    };

public:
    NO_COPY_SEMANTIC(Stack);
    NO_MOVE_SEMANTIC(Stack);

    Stack() = default;
    ~Stack() = default;

    std::optional<Error> Push(Type value)
    {
        if (size_ >= capacity) {
            return Error::OVERFLOW;
        }

        data_[size_++] = value;
        return std::nullopt;
    }

    std::optional<Error> Pop()
    {
        if (size_ == 0) {
            return Error::EMPTY_POP;
        }

        --size_;

        return std::nullopt;
    }

    std::pair<Type, std::optional<Error>> Top() const
    {
        if (size_ == 0) {
            return {0, Error::EMPTY_TOP};
        }

        return {data_[size_ - 1], std::nullopt};
    }

    size_t GetSize() const
    {
        return size_;
    }

    bool Empty() const
    {
        return size_ == 0;
    }

private:
    Type data_[capacity];
    size_t size_ {0};
};

} // namespace rvsim::utils

#endif // SIMULATOR_COMMON_UTILS_STACK_H
