#include "hart/hart.h"
#include "hart/basic_block_manager.h"
#include <iostream>
namespace rvsim {

std::pair<BasicBlock *, Exception> BasicBlockManager::GetNextBB()
{
    size_t start_pc = hart_->GetPC();

    BasicBlock *bb = bb_cache_.GetBBFromCache(start_pc);
    if (bb != nullptr) {
        return std::make_pair(bb, Exception::NONE);
    }

    auto bb_unique = std::make_unique<BasicBlock>();
    bb = bb_unique.get();
    basic_blocks_.push_back(std::move(bb_unique));

    bb->SetStartPC(start_pc);
    // Set bb into cache only after start pc has been setted
    bb_cache_.SetBBIntoCache(bb);

    instr_size_t raw_instr = 0;
    size_t curr_pc = start_pc;

    for (size_t instr_counter = 0;; ++instr_counter) {
        Exception fetch_exception = hart_->FetchInstruction(&raw_instr);
        if (UNLIKELY(fetch_exception != Exception::NONE)) {
            return std::make_pair(nullptr, fetch_exception);
        }

        Instruction *instr = bb->CreateEmptyInstruction();
        assert(instr != nullptr);

        hart_->DecodeInstruction(instr, raw_instr);
        if (UNLIKELY(instr->id == InstructionId::INVALID_ID)) {
            std::make_pair(nullptr, Exception::INVALID_INSTRUCTION_TYPE);
        }

        if (instr->IsBranch()) {
            hart_->SetPC(start_pc);
            // Jump is the last instruction in the basic block
            return std::make_pair(bb, Exception::NONE);
        }
        if (instr_counter == BasicBlock::GetBBMaxSize() - 1) {
            hart_->SetPC(start_pc);
            bb->CreateBBEndInstruction();
            return std::make_pair(bb, Exception::NONE);
        }

        curr_pc += sizeof(instr_size_t);
        hart_->SetPC(curr_pc);
    }

    return std::make_pair(nullptr, Exception::NONE);
}

} // namespace rvsim
