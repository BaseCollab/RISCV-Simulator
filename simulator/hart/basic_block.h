#ifndef SIMULATOR_HART_BASIC_BLOCK_H
#define SIMULATOR_HART_BASIC_BLOCK_H

#include "common/macros.h"
#include "common/constants.h"
#include "common/config.h"

#include "hart/instruction/instruction.h"
#include <memory>
#include <vector>
#include <cassert>

namespace rvsim {

class Hart;

class BasicBlock {
public:
    NO_COPY_SEMANTIC(BasicBlock);
    NO_MOVE_SEMANTIC(BasicBlock);

    BasicBlock() = default;
    ~BasicBlock() = default;

    void SetStartPC(size_t pc)
    {
        start_pc_ = pc;
    }

    size_t GetStartPC() const
    {
        return start_pc_;
    }

    Instruction *AddInstruction()
    {
        return &(instructions_.emplace_back(Instruction()));
    }

    const std::vector<Instruction> &GetInstructions() const
    {
        return instructions_;
    }

private:
    // pc in the file from which this basic block starts
    size_t start_pc_ {0};
    // size of basic block in bytes
    size_t size_ {0};

    std::vector<Instruction> instructions_;
};

template <size_t CACHE_SIZE>
class BasicBlockCache {
public:
    NO_COPY_SEMANTIC(BasicBlockCache);
    NO_MOVE_SEMANTIC(BasicBlockCache);

    explicit BasicBlockCache() : cache_(CACHE_SIZE) {}

    ~BasicBlockCache() = default;

    BasicBlock *GetBBFromCache(size_t pc) const
    {
        BasicBlock *bb = cache_[pc % CACHE_SIZE];
        if (bb == nullptr) {
            return nullptr;
        }
        // If pc not equal to start pc of basic block then in cache_ on pc % CACHE_SIZE index
        // there is another base block that will be pushed out after calling SetBBIntoCache()
        return (bb->GetStartPC() == pc) ? bb : nullptr;
    }

    void SetBBIntoCache(BasicBlock *bb)
    {
        assert(bb != nullptr);
        size_t start_pc = bb->GetStartPC();
        // It may happen that cache_ already has an entry in the start_pc % CACHE_SIZE index
        // In this case entry just pushed out in favor of a new one
        cache_[start_pc % CACHE_SIZE] = bb;
    }

private:
    // <start_pc, BasicBlock>
    std::vector<BasicBlock *> cache_;
};

class BasicBlockManager {
public:
    static constexpr size_t DEFAULT_BB_CACHE_SIZE = 1 << 8;

public:
    NO_COPY_SEMANTIC(BasicBlockManager);
    NO_MOVE_SEMANTIC(BasicBlockManager);

    explicit BasicBlockManager(Hart *hart) : hart_(hart) {};

    ~BasicBlockManager() = default;

    BasicBlock *GetNextBB();

private:
    Hart *hart_ {nullptr};

    std::vector<std::unique_ptr<BasicBlock>> basic_blocks_;

    BasicBlockCache<DEFAULT_BB_CACHE_SIZE> bb_cache_;
};

} // namespace rvsim

#endif // SIMULATOR_HART_BASIC_BLOCK_H
