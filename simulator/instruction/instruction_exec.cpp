/*
 * This file is autogenerated by 'generate.py' script from risc-v.yaml file
 * This file can be changed without additional autogeneration
 ***********************************************************
 * Written by Glaz Roman (Vokerlee)
 */

#include "instruction_exec.h"

namespace rvsim {
namespace iexec {

void BEQ(Hart &hart, const Instruction &instr)
{
    if (hart.GetGPR(instr.GetRS1()) == hart.GetGPR(instr.GetRS2()))
        hart.SetPCTarget(hart.GetPC() + instr.GetIMM());
    else
        hart.SetPCTarget(hart.GetPC() + sizeof(instr_size_t));
}

void BNE(Hart &hart, const Instruction &instr)
{
    if (hart.GetGPR(instr.GetRS1()) != hart.GetGPR(instr.GetRS2()))
        hart.SetPCTarget(hart.GetPC() + instr.GetIMM());
    else
        hart.SetPCTarget(hart.GetPC() + sizeof(instr_size_t));
}

void BLT(Hart &hart, const Instruction &instr)
{
    auto rs1 = bitops::MakeSigned(hart.GetGPR(instr.GetRS1()));
    auto rs2 = bitops::MakeSigned(hart.GetGPR(instr.GetRS2()));

    if (rs1 < rs2)
        hart.SetPCTarget(hart.GetPC() + instr.GetIMM());
    else
        hart.SetPCTarget(hart.GetPC() + sizeof(instr_size_t));
}

void BGE(Hart &hart, const Instruction &instr)
{
    auto rs1 = bitops::MakeSigned(hart.GetGPR(instr.GetRS1()));
    auto rs2 = bitops::MakeSigned(hart.GetGPR(instr.GetRS2()));

    if (rs1 >= rs2)
        hart.SetPCTarget(hart.GetPC() + instr.GetIMM());
    else
        hart.SetPCTarget(hart.GetPC() + sizeof(instr_size_t));
}

void BLTU(Hart &hart, const Instruction &instr)
{
    auto rs1 = hart.GetGPR(instr.GetRS1());
    auto rs2 = hart.GetGPR(instr.GetRS2());

    if (rs1 < rs2)
        hart.SetPCTarget(hart.GetPC() + instr.GetIMM());
    else
        hart.SetPCTarget(hart.GetPC() + sizeof(instr_size_t));
}

void BGEU(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_BGEU(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void JALR(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_JALR(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void JAL(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_JAL(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void LUI(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_LUI(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void AUIPC(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_AUIPC(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void ADDI(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_ADDI(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void SLLI(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_SLLI(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void SLTI(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_SLTI(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void SLTIU(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_SLTIU(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void XORI(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_XORI(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void SRLI(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_SRLI(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void SRAI(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_SRAI(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void ORI(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_ORI(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void ANDI(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_ANDI(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void ADD(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_ADD(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void SUB(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_SUB(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void SLL(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_SLL(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void SLT(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_SLT(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void SLTU(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_SLTU(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void XOR(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_XOR(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void SRL(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_SRL(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void SRA(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_SRA(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void OR(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_OR(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void AND(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_AND(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void ADDIW(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_ADDIW(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void SLLIW(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_SLLIW(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void SRLIW(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_SRLIW(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void SRAIW(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_SRAIW(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void ADDW(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_ADDW(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void SUBW(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_SUBW(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void SLLW(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_SLLW(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void SRLW(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_SRLW(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void SRAW(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_SRAW(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void LB(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_LB(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void LH(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_LH(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void LW(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_LW(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void LD(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_LD(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void LBU(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_LBU(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void LHU(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_LHU(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void LWU(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_LWU(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void SB(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_SB(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void SH(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_SH(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void SW(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_SW(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void SD(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_SD(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FENCE(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FENCE(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FENCE_I(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FENCE_I(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void MUL(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_MUL(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void MULH(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_MULH(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void MULHSU(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_MULHSU(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void MULHU(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_MULHU(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void DIV(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_DIV(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void DIVU(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_DIVU(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void REM(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_REM(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void REMU(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_REMU(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void MULW(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_MULW(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void DIVW(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_DIVW(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void DIVUW(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_DIVUW(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void REMW(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_REMW(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void REMUW(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_REMUW(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void AMOADD_W(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_AMOADD_W(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void AMOXOR_W(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_AMOXOR_W(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void AMOOR_W(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_AMOOR_W(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void AMOAND_W(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_AMOAND_W(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void AMOMIN_W(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_AMOMIN_W(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void AMOMAX_W(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_AMOMAX_W(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void AMOMINU_W(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_AMOMINU_W(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void AMOMAXU_W(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_AMOMAXU_W(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void AMOSWAP_W(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_AMOSWAP_W(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void LR_W(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_LR_W(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void SC_W(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_SC_W(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void AMOADD_D(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_AMOADD_D(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void AMOXOR_D(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_AMOXOR_D(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void AMOOR_D(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_AMOOR_D(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void AMOAND_D(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_AMOAND_D(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void AMOMIN_D(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_AMOMIN_D(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void AMOMAX_D(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_AMOMAX_D(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void AMOMINU_D(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_AMOMINU_D(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void AMOMAXU_D(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_AMOMAXU_D(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void AMOSWAP_D(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_AMOSWAP_D(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void LR_D(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_LR_D(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void SC_D(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_SC_D(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void ECALL(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_ECALL(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void EBREAK(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_EBREAK(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void URET(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_URET(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void SRET(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_SRET(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void MRET(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_MRET(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void DRET(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_DRET(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void SFENCE_VMA(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_SFENCE_VMA(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void WFI(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_WFI(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void CSRRW(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_CSRRW(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void CSRRS(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_CSRRS(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void CSRRC(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_CSRRC(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void CSRRWI(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_CSRRWI(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void CSRRSI(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_CSRRSI(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void CSRRCI(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_CSRRCI(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void HFENCE_VVMA(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_HFENCE_VVMA(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void HFENCE_GVMA(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_HFENCE_GVMA(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FADD_S(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FADD_S(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FSUB_S(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FSUB_S(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FMUL_S(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FMUL_S(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FDIV_S(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FDIV_S(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FSGNJ_S(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FSGNJ_S(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FSGNJN_S(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FSGNJN_S(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FSGNJX_S(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FSGNJX_S(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FMIN_S(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FMIN_S(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FMAX_S(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FMAX_S(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FSQRT_S(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FSQRT_S(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FADD_D(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FADD_D(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FSUB_D(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FSUB_D(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FMUL_D(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FMUL_D(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FDIV_D(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FDIV_D(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FSGNJ_D(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FSGNJ_D(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FSGNJN_D(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FSGNJN_D(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FSGNJX_D(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FSGNJX_D(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FMIN_D(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FMIN_D(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FMAX_D(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FMAX_D(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FCVT_S_D(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FCVT_S_D(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FCVT_D_S(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FCVT_D_S(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FSQRT_D(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FSQRT_D(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FADD_Q(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FADD_Q(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FSUB_Q(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FSUB_Q(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FMUL_Q(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FMUL_Q(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FDIV_Q(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FDIV_Q(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FSGNJ_Q(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FSGNJ_Q(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FSGNJN_Q(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FSGNJN_Q(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FSGNJX_Q(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FSGNJX_Q(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FMIN_Q(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FMIN_Q(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FMAX_Q(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FMAX_Q(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FCVT_S_Q(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FCVT_S_Q(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FCVT_Q_S(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FCVT_Q_S(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FCVT_D_Q(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FCVT_D_Q(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FCVT_Q_D(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FCVT_Q_D(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FSQRT_Q(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FSQRT_Q(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FLE_S(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FLE_S(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FLT_S(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FLT_S(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FEQ_S(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FEQ_S(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FLE_D(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FLE_D(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FLT_D(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FLT_D(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FEQ_D(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FEQ_D(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FLE_Q(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FLE_Q(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FLT_Q(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FLT_Q(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FEQ_Q(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FEQ_Q(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FCVT_W_S(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FCVT_W_S(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FCVT_WU_S(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FCVT_WU_S(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FCVT_L_S(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FCVT_L_S(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FCVT_LU_S(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FCVT_LU_S(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FMV_X_W(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FMV_X_W(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FCLASS_S(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FCLASS_S(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FCVT_W_D(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FCVT_W_D(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FCVT_WU_D(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FCVT_WU_D(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FCVT_L_D(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FCVT_L_D(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FCVT_LU_D(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FCVT_LU_D(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FMV_X_D(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FMV_X_D(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FCLASS_D(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FCLASS_D(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FCVT_W_Q(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FCVT_W_Q(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FCVT_WU_Q(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FCVT_WU_Q(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FCVT_L_Q(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FCVT_L_Q(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FCVT_LU_Q(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FCVT_LU_Q(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FMV_X_Q(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FMV_X_Q(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FCLASS_Q(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FCLASS_Q(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FCVT_S_W(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FCVT_S_W(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FCVT_S_WU(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FCVT_S_WU(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FCVT_S_L(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FCVT_S_L(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FCVT_S_LU(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FCVT_S_LU(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FMV_W_X(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FMV_W_X(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FCVT_D_W(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FCVT_D_W(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FCVT_D_WU(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FCVT_D_WU(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FCVT_D_L(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FCVT_D_L(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FCVT_D_LU(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FCVT_D_LU(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FMV_D_X(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FMV_D_X(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FCVT_Q_W(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FCVT_Q_W(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FCVT_Q_WU(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FCVT_Q_WU(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FCVT_Q_L(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FCVT_Q_L(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FCVT_Q_LU(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FCVT_Q_LU(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FMV_Q_X(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FMV_Q_X(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FLW(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FLW(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FLD(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FLD(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FLQ(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FLQ(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FSW(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FSW(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FSD(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FSD(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FSQ(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FSQ(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FMADD_S(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FMADD_S(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FMSUB_S(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FMSUB_S(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FNMSUB_S(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FNMSUB_S(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FNMADD_S(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FNMADD_S(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FMADD_D(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FMADD_D(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FMSUB_D(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FMSUB_D(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FNMSUB_D(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FNMSUB_D(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FNMADD_D(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FNMADD_D(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FMADD_Q(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FMADD_Q(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FMSUB_Q(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FMSUB_Q(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FNMSUB_Q(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FNMSUB_Q(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

void FNMADD_Q(Hart &hart, const Instruction &instr)
{
    std::cerr << "function exec_FNMADD_Q(Hart &hart, const Instruction &instr) is not implemented yet!" << std::endl;
}

} // namespace iexec

} // namespace rvsim
