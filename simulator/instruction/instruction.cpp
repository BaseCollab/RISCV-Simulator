#include "instruction.h"

// clang-format off

namespace rvsim {

Instruction::Instruction(instr_size_t instr)
{
    Decode(instr);
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

gpr_idx_t Instruction::GetRS1() const
{
    return rs1_;
}

gpr_idx_t Instruction::GetRS2() const
{
    return rs2_;
}

gpr_idx_t Instruction::GetRS3() const
{
    return rs3_;
}

gpr_idx_t Instruction::GetRD() const
{
    return rd_;
}

gpr_idx_t Instruction::GetRM() const
{
    return rm_;
}

word_t Instruction::GetIMM() const
{
    return imm_;
}

const instr_attrs_t &Instruction::GetAttrs() const
{
    return attributes_;
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
