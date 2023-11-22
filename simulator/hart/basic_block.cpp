#include "hart/basic_block.h"
#include "hart/hart.h"

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
    instr_size_t raw_instr = 0;

    size_t curr_pc = start_pc;

    while (true) {
        hart_->FetchInstruction(&raw_instr);

        Instruction *instr = bb->AddInstruction();
        hart_->DecodeInstruction(instr, raw_instr);
        instr->pc = curr_pc;

        if (instr->IsBranch()) {
            hart_->SetPC(start_pc);
            // Jump is the last instruction in the basic block
            return bb;
        }

        curr_pc += sizeof(instr_size_t);
        hart_->SetPC(curr_pc);
    }

    return nullptr;
}

} // namespace rvsim
