#include "sim_jit/sim_generator.h"

#include "hart/hart.h"

namespace rvsim {

using namespace asmjit;

void Generator::Initialize()
{
    static auto entry_signature = FuncSignatureT<void, Hart *, const Instruction *>();
    asmjit::_abi_1_11::FuncNode *entry_node = compiler_.addFunc(entry_signature);

    hart_p_ = compiler_.newGpq();
    instr_p_ = compiler_.newGpq();
    entry_node->setArg(0, hart_p_);
    entry_node->setArg(1, instr_p_);

    pc_p_ = compiler_.newGpq();
    compiler_.mov(pc_p_, hart_p_);
    compiler_.add(pc_p_, Hart::GetPcOffset());

    regs_p_ = compiler_.newGpq();
    compiler_.mov(regs_p_, hart_p_);
    compiler_.add(regs_p_, Hart::GetGPROffset());
}

void Generator::Finalize()
{
    compiler_.endFunc();
    compiler_.finalize();
}

x86::Gp Generator::GenerateGetReg(size_t index)
{
    asmjit::_abi_1_11::x86::Gp reg = compiler_.newGpq();
    compiler_.mov(reg, x86::qword_ptr(regs_p_, sizeof(reg_t) * index));
    return reg;
}

void Generator::GenerateSetReg(size_t index, uint64_t imm)
{
    if (index > 0) {
        compiler_.mov(x86::qword_ptr(regs_p_, sizeof(reg_t) * index), imm);
    }
}

void Generator::GenerateSetReg(size_t index, x86::Gp reg)
{
    if (index > 0) {
        compiler_.mov(x86::qword_ptr(regs_p_, sizeof(reg_t) * index), reg);
    }
}

x86::Gp Generator::GenerateGetPC()
{
    asmjit::_abi_1_11::x86::Gp pc = compiler_.newGpq();
    compiler_.mov(pc, x86::qword_ptr(pc_p_));
    return pc;
}

void Generator::GenerateSetPC(uint64_t imm)
{
    compiler_.mov(x86::qword_ptr(pc_p_), imm);
}

void Generator::GenerateSetPC(x86::Gp pc)
{
    compiler_.mov(x86::qword_ptr(pc_p_), pc);
}

void Generator::GenerateIncrementPC()
{
    compiler_.add(x86::qword_ptr(pc_p_), sizeof(instr_size_t));
}

void Generator::GenerateDefaultCall(iexec::iexec_type executor, size_t instr_offest)
{
    asmjit::_abi_1_11::x86::Gp instr = compiler_.newGpq();
    compiler_.mov(instr, instr_p_);
    compiler_.add(instr, sizeof(Instruction) * instr_offest);

    static auto executor_signature = FuncSignatureT<void, Hart *, const Instruction &>();
    asmjit::InvokeNode *invokeNode = nullptr;
    compiler_.invoke(&invokeNode, executor, executor_signature);
    invokeNode->setArg(0, hart_p_);
    invokeNode->setArg(1, instr);
}

void Generator::GenerateADD(const Instruction &instr)
{
    asmjit::x86::Gp op1 = GenerateGetReg(instr.rs1);
    asmjit::x86::Gp op2 = GenerateGetReg(instr.rs2);
    compiler_.add(op1, op2);
    GenerateSetReg(instr.rd, op1);
    GenerateIncrementPC();
}

} // namespace rvsim