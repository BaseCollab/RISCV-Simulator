#ifndef SIMULATOR_MEMORY_PAGES_CONTROLLER_H
#define SIMULATOR_MEMORY_PAGES_CONTROLLER_H

#include "memory.h"
#include "constants.h"

namespace rvsim {

class PagesController {
public:
    enum class Error {
        NONE = -1,
        OUT_OF_PAGES = 0
    };

public:
    NO_COPY_SEMANTIC(PagesController);
    NO_MOVE_SEMANTIC(PagesController);

    PagesController()
    {
        for (size_t i = 0; i < PAGES_NMB; ++i) {
            clean_pages_stack_.Push(i);
        }
    }

    ~PagesController() = default;

    std::pair<uint16_t, std::optional<Error>> GetCleanPage()
    {
        if (clean_pages_stack_.Empty()) {
            return {0, Error::OUT_OF_PAGES};
        }

        auto [idx, err] = clean_pages_stack_.Pop();
        (void) err;

        pages_states_[idx] = 1;
        return {idx, std::nullopt};
    }

    size_t GetPageSize() const
    {
        return PAGE_SIZE;
    }

private:
    // TODO(R.Glaz): move PAGE_SIZE to hart
    static constexpr size_t PAGE_SIZE = 4 * KBYTE_SIZE;

    static constexpr size_t PAGES_NMB = Memory::GetMemorySize() / PAGE_SIZE;

    static_assert(PAGES_NMB == 8192); // 1 << 13

private:
    // For each pages we save state, 0 - page is clean, 1 - page is not clean
    uint8_t pages_states_[PAGES_NMB] {0};
    // Stack that constists only indexes of clean pages
    utils::Stack<uint16_t, PAGES_NMB> clean_pages_stack_;
};

} // namespace rvsim

#endif // SIMULATOR_MEMORY_PAGES_CONTROLLER_H
