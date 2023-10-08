#ifndef SIMULATOR_MEMORY_PAGES_CONTROLLER_H
#define SIMULATOR_MEMORY_PAGES_CONTROLLER_H

#include "common/constants.h"
#include "common/config.h"
#include "memory.h"

#include <iostream>
#include <memory>

namespace rvsim {

class PagesController {
public:
    // clang-format off
    enum class Error {
        NONE         = -1,
        OUT_OF_PAGES = 0
    };
    // clang-format on

public:
    NO_COPY_SEMANTIC(PagesController);
    NO_MOVE_SEMANTIC(PagesController);

    explicit PagesController(size_t pages_number) : pages_number_(pages_number)
    {
        pages_states_ = std::make_unique<uint8_t[]>(pages_number_);
        clean_pages_stack_ = std::make_unique<utils::Stack<size_t>>(pages_number_);

        InitializePagesStack();
    }

    ~PagesController() = default;

    void InitializePagesStack()
    {
        // Stack is constant size
        for (long i = pages_number_ - 1; i >= 0; --i) {
            clean_pages_stack_->Push(i);
        }
    }

    std::pair<reg_t, std::optional<Error>> GetCleanPage()
    {
        if (clean_pages_stack_->Empty()) {
            return {0, Error::OUT_OF_PAGES};
        }

        auto [idx, top_err] = clean_pages_stack_->Top();
        if (top_err.has_value()) {
            return {0, Error::OUT_OF_PAGES};
        }

        auto pop_err = clean_pages_stack_->Pop();
        if (pop_err.has_value()) {
            return {0, Error::OUT_OF_PAGES};
        }

        pages_states_[idx] = 1;
        return {idx, std::nullopt};
    }

private:
    // For each pages we save state, 0 - page is clean, 1 - page is not clean
    std::unique_ptr<uint8_t[]> pages_states_;
    // Stack that constists only indexes of clean pages
    std::unique_ptr<utils::Stack<size_t>> clean_pages_stack_;

    size_t pages_number_ {0};
};

} // namespace rvsim

#endif // SIMULATOR_MEMORY_PAGES_CONTROLLER_H
