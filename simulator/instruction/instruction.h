#ifndef SIMULATOR_INSTRUCTION_INSTRUCTION_H
#define SIMULATOR_INSTRUCTION_INSTRUCTION_H

#include "common/constants.h"
#include "common/macros.h"
#include "common/config.h"

#include "instruction_id.h"

#include <cstdint>

namespace rvsim {

// clang-format off

class Instruction {
public:
    struct instr_attrs_t {
        bool is_branch = false;
        bool is_load   = false;
        bool is_store  = false;
        bool is_pseudo = false; // for plugins support

        Mode mode{Mode::USER_MODE};
    };

public:
    NO_COPY_SEMANTIC(Instruction);
    NO_MOVE_SEMANTIC(Instruction);

    Instruction() = default;
    explicit Instruction(instr_size_t instr);
    ~Instruction() = default;

    bool IsBranch() const;
    bool IsLoad()   const;
    bool IsStore()  const;
    bool IsPseudo() const;

    InstructionId GetId() const;

    void Clear();
    void Decode(instr_size_t instr);

private:
    gpr_idx_t rs1_{0};
    gpr_idx_t rs2_{0};
    gpr_idx_t rs3_{0};

    gpr_idx_t rd_{0};

    gpr_idx_t rm_{0};

    word_t imm_{0};

    instr_attrs_t attributes_;

    InstructionId id_ = InstructionId::INVALID_ID;
};

// clang-format on

} // namespace rvsim

#endif // SIMULATOR_INSTRUCTION_INSTRUCTION_H
