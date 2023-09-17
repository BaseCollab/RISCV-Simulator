#include "instruction.h"

// clang-format off

namespace rvsim {

Insn::Insn(insn_size_t insn)
{
    Decode(insn);
}

bool Insn::IsBranch() const
{
    return attributes_.is_branch;
}

bool Insn::IsLoad() const
{
    return attributes_.is_load;
}

bool Insn::IsStore() const
{
    return attributes_.is_store;
}

bool Insn::IsPseudo() const
{
    return attributes_.is_pseudo;
}

InsnId Insn::Id() const
{
    return id_;
}

void Insn::Clear()
{
    attributes_.is_branch = false;
    attributes_.is_load   = false;
    attributes_.is_store  = false;
    attributes_.mode      = Mode::USER_MODE;

    id_ = InsnId::INVALID_ID;

    rs1_ = rs2_ = rs3_ = rd_ = rm_ = 0;

    imm_ = 0;
}

// clang-format on

} // namespace rvsim
