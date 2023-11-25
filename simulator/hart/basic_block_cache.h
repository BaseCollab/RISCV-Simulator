#ifndef SIMULATOR_HART_BASIC_BLOCK_CACHE_H
#define SIMULATOR_HART_BASIC_BLOCK_CACHE_H

#include "common/macros.h"
#include "common/config.h"
#include "common/constants.h"

#include <cassert>

namespace rvsim {

template <size_t CACHE_SIZE>
class BasicBlockCache {
public:
    NO_COPY_SEMANTIC(BasicBlockCache);
    NO_MOVE_SEMANTIC(BasicBlockCache);

    BasicBlockCache() = default;
    ~BasicBlockCache() = default;

    void SetBBIntoCache(BasicBlock *bb)
    {
        assert(bb != nullptr);
        size_t start_pc = bb->GetStartPC();
        // It may happen that cache_ already has an entry in the start_pc >> 2 % CACHE_SIZE index.
        // In this case entry just pushed out in favor of a new one.
        cache_[(start_pc >> 2) % CACHE_SIZE] = bb;
    }

    BasicBlock *GetBBFromCache(size_t pc) const
    {
        BasicBlock *bb = cache_[(pc >> 2) % CACHE_SIZE];
        if (bb == nullptr) {
            return nullptr;
        }
        // If pc not equal to start pc of basic block then in cache_ on pc >> 2 % CACHE_SIZE index
        // there is another base block that will be pushed out after calling SetBBIntoCache().
        return (bb->GetStartPC() == pc) ? bb : nullptr;
    }

private:
    // PC is always a multiple of 4, so the last 2 bits are zero
    // { start_pc >> 2, BasicBlock }
    std::array<BasicBlock *, CACHE_SIZE> cache_;
};

} // namespace rvsim

#endif // SIMULATOR_HART_BASIC_BLOCK_CACHE_H
