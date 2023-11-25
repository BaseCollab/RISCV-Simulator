#include "hart/hart.h"
#include "hart/basic_block_manager.h"
#include <iostream>
namespace rvsim {

BasicBlock *BasicBlockManager::GetNextBB()
{
    size_t start_pc = hart_->GetPC();

    BasicBlock *bb = bb_cache_.GetBBFromCache(start_pc);
    if (bb != nullptr) {
        return bb;
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
        hart_->FetchInstruction(&raw_instr);

        Instruction *instr = bb->CreateEmptyInstruction();
        assert(instr != nullptr);

        hart_->DecodeInstruction(instr, raw_instr);

        if (instr->IsBranch()) {
            hart_->SetPC(start_pc);
            // Jump is the last instruction in the basic block
            return bb;
        }
        if (instr_counter == BasicBlock::GetBBMaxSize() - 1) {
            hart_->SetPC(start_pc);
            bb->CreateBBEndInstruction();
            return bb;
        }

        curr_pc += sizeof(instr_size_t);
        hart_->SetPC(curr_pc);
    }

    return nullptr;
}

} // namespace rvsim
