#include "instruction.h"

// clang-format off

namespace rvsim {

Insn::Insn(word_t cmd)
{
    Decode(cmd);
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

const InsnId Insn::Id() const
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

    rs1_ = rs2_ = rd_ = 0;

    imm_ = 0;
}

void Insn::Decode(word_t cmd)
{
    cmd++; // not implemented yet
}

// clang-format on

} // namespace rvsim
