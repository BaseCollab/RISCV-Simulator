#ifndef SIMULATOR_INSTRUCTION_INSTRUCTION_H
#define SIMULATOR_INSTRUCTION_INSTRUCTION_H

#include "common/constants.h"
#include "common/macros.h"
#include "common/config.h"

#include "instruction_id.h"

#include <cstdint>
#include <type_traits>

namespace rvsim {

// clang-format off

struct Instruction {
public:
    struct instr_attrs_t {
        bool is_branch = false;
        bool is_load   = false;
        bool is_store  = false;
        bool is_pseudo = false; // for plugins support

        Mode mode{Mode::USER_MODE};
    };

public:
    size_t pc {0};

    gpr_idx_t rs1 {0};
    gpr_idx_t rs2 {0};
    gpr_idx_t rs3 {0};

    gpr_idx_t rd {0};

    gpr_idx_t rm {0};

    std::make_signed_t<word_t> imm {0};

    instr_attrs_t attributes;

    InstructionId id = InstructionId::INVALID_ID;

public:
    DEFAULT_COPY_SEMANTIC(Instruction);
    DEFAULT_MOVE_SEMANTIC(Instruction);

    Instruction() = default;
    ~Instruction() = default;

    bool IsBranch() const
    {
        return attributes.is_branch;
    }

    bool IsLoad() const
    {
        return attributes.is_load;
    }

    bool IsStore() const
    {
        return attributes.is_store;
    }

    bool IsPseudo() const
    {
        return attributes.is_pseudo;
    }

    void Clear()
    {
        attributes.is_branch = false;
        attributes.is_load   = false;
        attributes.is_store  = false;
        attributes.mode      = Mode::USER_MODE;

        id = InstructionId::INVALID_ID;

        rs1 = rs2 = rs3 = rd = rm = 0;

        imm = 0;
    }
};

// clang-format on

} // namespace rvsim

#endif // SIMULATOR_INSTRUCTION_INSTRUCTION_H
