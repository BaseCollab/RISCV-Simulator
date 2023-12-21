#include "sim_jit/sim_generator.h"

#include "hart/hart.h"

namespace rvsim {

void Generator::Initialize()
{
    static auto entry_signature = asmjit::_abi_1_11::FuncSignatureT<void, Hart *, const Instruction *>();
    asmjit::_abi_1_11::FuncNode *entry_node = compiler_.addFunc(entry_signature);

    hart_p_ = compiler_.newGpq();
    instr_p_ = compiler_.newGpq();
    entry_node->setArg(0, hart_p_);
    entry_node->setArg(1, instr_p_);

    pc_p_ = compiler_.newGpq();
    compiler_.mov(pc_p_, hart_p_);
    compiler_.add(pc_p_, Hart::GetPCOffset());

    regs_p_ = compiler_.newGpq();
    compiler_.mov(regs_p_, hart_p_);
    compiler_.add(regs_p_, Hart::GetGPROffset());
}

void Generator::Finalize()
{
    compiler_.endFunc();
    compiler_.finalize();
}

asmjit::_abi_1_11::x86::Gp Generator::GenerateGetReg(size_t index)
{
    asmjit::_abi_1_11::x86::Gp reg = compiler_.newGpq();
    compiler_.mov(reg, asmjit::_abi_1_11::x86::qword_ptr(regs_p_, sizeof(reg_t) * index));
    return reg;
}

void Generator::GenerateDefaultCall(iexec::iexec_type executor, size_t instr_offest)
{
    asmjit::_abi_1_11::x86::Gp instr = compiler_.newGpq();
    compiler_.mov(instr, instr_p_);
    compiler_.add(instr, sizeof(Instruction) * instr_offest);

    static auto executor_signature = asmjit::_abi_1_11::FuncSignatureT<void, Hart *, const Instruction &>();
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
    
    // set register
    if (instr.rd > 0) {
        compiler_.mov(asmjit::_abi_1_11::x86::qword_ptr(regs_p_, sizeof(reg_t) * instr.rd), op1);
    }
    
    // inc PC
    compiler_.add(asmjit::_abi_1_11::x86::qword_ptr(pc_p_), sizeof(instr_size_t));

}

} // namespace rvsim