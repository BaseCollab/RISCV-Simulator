#ifndef GENERATOR_H
#define GENERATOR_H


// Disable warning because the function uses computed goto
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-label-as-value"
#pragma clang diagnostic ignored "-Wshadow"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wshadow"
#endif

#include "../asmjit/src/asmjit/asmjit.h"

// Disable warning because the function uses computed goto
#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#include "hart/hart.h"
#include "hart/instruction/instruction_exec_inl.h"

namespace rvsim {

class Compiler;

class Generator {
public:
    Generator(asmjit::CodeHolder *code) : compiler_(code) {}

    void Initialize();
    void Finalize();

    void GenerateDefaultCall(iexec::iexec_type executor, size_t instr_offset);
    void GenerateADD(const Instruction &instr);


private:

    asmjit::x86::Gp GenerateGetReg(size_t index);
    void GenerateSetReg(size_t index, uint64_t imm);
    void GenerateSetReg(size_t index, asmjit::x86::Gp reg);

    asmjit::x86::Gp GenerateGetPC();
    void GenerateSetPC(uint64_t imm);
    void GenerateSetPC(asmjit::x86::Gp reg);
    void GenerateIncrementPC();

    void GeneratePrint(const char *str, asmjit::x86::Gp reg);

    asmjit::x86::Compiler compiler_;
    asmjit::x86::Gp hart_p_;
    asmjit::x86::Gp pc_p_;
    asmjit::x86::Gp regs_p_;
    asmjit::x86::Gp instr_p_;
};

}  // namespace rvsim

#endif  // GENERATOR_H