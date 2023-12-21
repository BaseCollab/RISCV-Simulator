#ifndef SIMULATOR_HART_BASIC_BLOCK_H
#define SIMULATOR_HART_BASIC_BLOCK_H

#include "common/macros.h"
#include "common/config.h"
#include "common/constants.h"

#include "hart/instruction/instruction.h"

#include <array>

namespace rvsim {

class Hart;


class BasicBlock {
public:
    // +1 is necessary in case of trim the basic block with the BB_END instruction.
    using InstrArray = std::array<Instruction, BASIC_BLOCK_MAX_SIZE + 1>;

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

    Instruction *CreateEmptyInstruction()
    {
        instructions_[size_] = Instruction();
        return &(instructions_[size_++]);
    }

    Instruction *CreateBBEndInstruction()
    {
        instructions_[size_] = Instruction(InstructionId::BB_END);
        return &(instructions_[size_++]);
    }

    size_t GetSize() const
    {
        return size_;
    }

    const InstrArray &GetInstructions() const
    {
        return instructions_;
    }

    static constexpr size_t GetBBMaxSize()
    {
        return BASIC_BLOCK_MAX_SIZE;
    }

    size_t GetHotness() const { return hotness_; }
    void IncHotness() { ++hotness_; }
    void Execute(Hart* hart) { entry_(hart, instructions_.data()); }

private:
    // PC in ELF file from which this basic block starts.
    size_t start_pc_ {0};

    size_t size_ {0};

    InstrArray instructions_;

    using JIT_entry = void (*)(Hart *, const Instruction *);
    size_t hotness_ {0};
    JIT_entry entry_ {nullptr};
};

} // namespace rvsim

#endif // SIMULATOR_HART_BASIC_BLOCK_H
