#ifndef SIMULATOR_COMMON_UTILS_STACK_H
#define SIMULATOR_COMMON_UTILS_STACK_H

#include "common/macros.h"

#include <cstddef>
#include <optional>

namespace rvsim::utils {

template<typename Type, size_t capacity>
class Stack {
public:
    enum class Error {
        NONE = -1,
        OVERFLOW = 0,
        EMPTY_POP = 1
    };

public:
    NO_COPY_SEMANTIC(Stack);
    NO_MOVE_SEMANTIC(Stack);

    Stack()  = default;
    ~Stack() = default;

    std::optional<Error> Push(Type value)
    {
        if (size_ >= capacity) {
            return Error::OVERFLOW;
        }

        data_[size_++] = value;
    }

    std::pair<Type, std::optional<Error>> Pop()
    {
        if (size_ == 0) {
            return {0, Error::EMPTY_POP};
        }

        return {data_[size_--], std::nullopt};
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
