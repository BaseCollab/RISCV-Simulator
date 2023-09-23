#include "instruction.h"

// clang-format off

namespace rvsim {

Instruction::Instruction(instr_size_t insn)
{
    Decode(insn);
}

bool Instruction::IsBranch() const
{
    return attributes_.is_branch;
}

bool Instruction::IsLoad() const
{
    return attributes_.is_load;
}

bool Instruction::IsStore() const
{
    return attributes_.is_store;
}

bool Instruction::IsPseudo() const
{
    return attributes_.is_pseudo;
}

InstructionId Instruction::GetId() const
{
    return id_;
}

void Instruction::Clear()
{
    attributes_.is_branch = false;
    attributes_.is_load   = false;
    attributes_.is_store  = false;
    attributes_.mode      = Mode::USER_MODE;

    id_ = InstructionId::INVALID_ID;

    rs1_ = rs2_ = rs3_ = rd_ = rm_ = 0;

    imm_ = 0;
}

// clang-format on

} // namespace rvsim
