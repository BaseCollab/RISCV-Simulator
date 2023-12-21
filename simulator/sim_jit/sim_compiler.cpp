#include "sim_jit/sim_compiler.h"

#include "sim_jit/sim_generator.h"
#include "hart/hart.h"

namespace rvsim {


void Compiler::CompileBasicBlock() {
    asmjit::CodeHolder code;
    code.init(runtime_.environment(), runtime_.cpuFeatures());
    Generator sim_gen(&code);
    sim_gen.Initialize();

    const BasicBlock::InstrArray & instrs = bb_->GetInstructions();
    size_t instr_number = bb_->GetSize();
    for (size_t i = 0; i < instr_number ; ++i) {
        if (instrs[i].id != InstructionId::BB_END) {
            GenerateInstr(sim_gen, instrs[i], i);
        }
    }

    sim_gen.Finalize();
    iexec::iexec_type entry = nullptr;
    runtime_.add(&entry, &code);
    // BB: check is BB in cache, 
    // if not, set to bb compiled entry;
}


iexec::iexec_type InstrIdToExecutor(rvsim::InstructionId id)
{
#define MAP_ID_TO_EXEC(id_name)     \
    case InstructionId::id_name:    \
        return iexec::id_name

    switch (id) {
        MAP_ID_TO_EXEC(LUI);
        MAP_ID_TO_EXEC(AUIPC);
        MAP_ID_TO_EXEC(JAL);
        MAP_ID_TO_EXEC(JALR);
        MAP_ID_TO_EXEC(BEQ);
        MAP_ID_TO_EXEC(BNE);
        MAP_ID_TO_EXEC(BLT);
        MAP_ID_TO_EXEC(BGE);
        MAP_ID_TO_EXEC(BLTU);
        MAP_ID_TO_EXEC(BGEU);
        MAP_ID_TO_EXEC(LB);
        MAP_ID_TO_EXEC(LH);
        MAP_ID_TO_EXEC(LW);
        MAP_ID_TO_EXEC(LD);
        MAP_ID_TO_EXEC(LBU);
        MAP_ID_TO_EXEC(LHU);
        MAP_ID_TO_EXEC(LWU);
        MAP_ID_TO_EXEC(SB);
        MAP_ID_TO_EXEC(SH);
        MAP_ID_TO_EXEC(SW);
        MAP_ID_TO_EXEC(SD);
        MAP_ID_TO_EXEC(ADDI);
        MAP_ID_TO_EXEC(SLLI);
        MAP_ID_TO_EXEC(SLTI);
        MAP_ID_TO_EXEC(SLTIU);
        MAP_ID_TO_EXEC(XORI);
        MAP_ID_TO_EXEC(SRLI);
        MAP_ID_TO_EXEC(SRAI);
        MAP_ID_TO_EXEC(ORI);
        MAP_ID_TO_EXEC(ANDI);
        MAP_ID_TO_EXEC(ADDIW);
        MAP_ID_TO_EXEC(SLLIW);
        MAP_ID_TO_EXEC(SRLIW);
        MAP_ID_TO_EXEC(SRAIW);
        MAP_ID_TO_EXEC(ADD);
        MAP_ID_TO_EXEC(SLL);
        MAP_ID_TO_EXEC(SLT);
        MAP_ID_TO_EXEC(SLTU);
        MAP_ID_TO_EXEC(XOR);
        MAP_ID_TO_EXEC(SRL);
        MAP_ID_TO_EXEC(OR);
        MAP_ID_TO_EXEC(AND);
        MAP_ID_TO_EXEC(SUB);
        MAP_ID_TO_EXEC(SRA);
        MAP_ID_TO_EXEC(ADDW);
        MAP_ID_TO_EXEC(SUBW);
        MAP_ID_TO_EXEC(SLLW);
        MAP_ID_TO_EXEC(SRLW);
        MAP_ID_TO_EXEC(SRAW);
        MAP_ID_TO_EXEC(FENCE);
        MAP_ID_TO_EXEC(ECALL);
        MAP_ID_TO_EXEC(EBREAK);
        MAP_ID_TO_EXEC(MUL);
        MAP_ID_TO_EXEC(MULH);
        MAP_ID_TO_EXEC(MULHSU);
        MAP_ID_TO_EXEC(MULHU);
        MAP_ID_TO_EXEC(DIV);
        MAP_ID_TO_EXEC(DIVU);
        MAP_ID_TO_EXEC(REM);
        MAP_ID_TO_EXEC(REMU);
        MAP_ID_TO_EXEC(MULW);
        MAP_ID_TO_EXEC(DIVW);
        MAP_ID_TO_EXEC(DIVUW);
        MAP_ID_TO_EXEC(REMW);
        MAP_ID_TO_EXEC(REMUW);
        default:
            fprintf(stderr, "ERR in func InstrIdToExecutor: no mapping for instr id %d\n", std::int32_t(id)); // print error
            return nullptr;
    }
}


void Compiler::GenerateInstr(Generator &codegen, const Instruction &instr, size_t instr_offset) {
    switch (instr.id) {
        case InstructionId::ADD:
            printf("[x] Generate Add\n");
            codegen.GenerateADD(instr);
            return;
        default:
            printf("[x] Generate Default for id: %d\n", std::int32_t(instr.id));
            codegen.GenerateDefaultCall(InstrIdToExecutor(instr.id), instr_offset);
            return;
    }
}

}  // namespace rvsim