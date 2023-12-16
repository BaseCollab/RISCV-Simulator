#ifndef SIMULATOR_HART_BASIC_BLOCK_MANAGER_H
#define SIMULATOR_HART_BASIC_BLOCK_MANAGER_H

#include "common/macros.h"
#include "common/config.h"
#include "common/constants.h"

#include "hart/basic_block.h"
#include "hart/basic_block_cache.h"

#include <vector>
#include <memory>

namespace rvsim {

class Hart;
class BasicBlock;

class BasicBlockManager {
public:
    NO_COPY_SEMANTIC(BasicBlockManager);
    NO_MOVE_SEMANTIC(BasicBlockManager);

    explicit BasicBlockManager(Hart *hart) : hart_(hart) {};

    ~BasicBlockManager() = default;

    std::pair<BasicBlock *, Exception> GetNextBB();

    static constexpr size_t GetCacheSize()
    {
        return BB_CACHE_SIZE;
    }

private:
    Hart *hart_ {nullptr};

    std::vector<std::unique_ptr<BasicBlock>> basic_blocks_;

    BasicBlockCache<BB_CACHE_SIZE> bb_cache_;
};

} // namespace rvsim

#endif // SIMULATOR_HART_BASIC_BLOCK_MANAGER_H
