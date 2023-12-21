#ifndef COMPILER_H
#define COMPILER_H

// Disable warning
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

// Disable warning
#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#include "hart/basic_block.h"

namespace rvsim {

class Hart;
class Generator;

class Compiler {
public:
    Compiler(Hart *hart) : hart_(hart) {}

    void SetBB(BasicBlock* bb) {
        bb_ = bb;
    }

    void CompileBasicBlock();
    void CompileBasicBlock(BasicBlock* bb) {
        SetBB(bb);
        CompileBasicBlock();
    }

    void GenerateInstr(Generator &codegen, const Instruction &instr, size_t instr_offset);

private:
    asmjit::JitRuntime runtime_;
    Hart *hart_;
    BasicBlock *bb_;

};

} // namespace rvsim

#endif  // GENERATOR_H