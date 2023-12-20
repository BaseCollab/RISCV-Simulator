#include "plugin/plugin.h"
#include "plugin/plugin_handler.h"

#include "time.h"

extern "C" void FuncForPlugin(rvsim::Plugin &plug, rvsim::PluginRegimes regime)
{
    switch (regime) {
        case rvsim::PluginRegimes::COSIM_RUN: /* initialization for cosimulation routine */
        {
            FILE *dump_file = fopen("cosim.trace", "a");
            if (!dump_file) {
                std::cerr << "can't open file for cosim\n";
                return;
            }

            const rvsim::Instruction *cur_instr = plug.GetCurInstr();
            if (cur_instr->id == rvsim::InstructionId::BB_END)
                return;
            rvsim::PluginHandler *handler = plug.GetHandler();
            fprintf(dump_file, "PC:%li \n", handler->GetPC());
            //fprintf(dump_file, "Id: %d, ", std::int32_t(cur_instr->id));
            fprintf(dump_file, "rd: %hhu, ", cur_instr->rd == rvsim::SINK_REG_IDX ? 0 : cur_instr->rd);
            fprintf(dump_file, "rs1: %hhu, ", cur_instr->rs1);
            fprintf(dump_file, "rs2: %hhu, ", cur_instr->rs2);
            fprintf(dump_file, "rs3: %hhu, ", cur_instr->rs3);
            fprintf(dump_file, "imm: %x, ", cur_instr->imm & (0xFFF));
            fprintf(dump_file, "rm: %hhu\n", cur_instr->rm);

            // fprintf(dump_file, "rd: %lu, ", handler->GetGPR(cur_instr->rd));
            // fprintf(dump_file, "rs1: %lu, ", handler->GetGPR(cur_instr->rs1));

            if (static_cast<uint32_t>(handler->GetGPR(cur_instr->rd)) & (1ULL << 31)) {
                fprintf(dump_file, "rd: %llu, ", static_cast<uint32_t>(handler->GetGPR(cur_instr->rd)) | 0xFFFFFFFF00000000ULL);
            } else {
                fprintf(dump_file, "rd: %u, ", static_cast<uint32_t>(handler->GetGPR(cur_instr->rd)));
            }

            if (static_cast<uint32_t>(handler->GetGPR(cur_instr->rs1)) & (1ULL << 31)) {
                fprintf(dump_file, "rs1: %llu, ",   static_cast<uint32_t>(handler->GetGPR(cur_instr->rs1)) | 0xFFFFFFFF00000000ULL);
            } else {
                fprintf(dump_file, "rs1: %u, ",   static_cast<uint32_t>(handler->GetGPR(cur_instr->rs1)));
            }

            fprintf(dump_file, "rs2: %lu, ", handler->GetGPR(cur_instr->rs2));
            fprintf(dump_file, "rs3: %lu, ", handler->GetGPR(cur_instr->rs3));
            fprintf(dump_file, "rm: %lu\n", handler->GetGPR(cur_instr->rm));

            fprintf(dump_file, "#################################\n");

            fclose(dump_file);
            break;
        }
        default:
            std::cerr << "Undefinied regime of Plugin: " << std::int32_t(regime) << std::endl;
            break;
    }
}
