#include "plugin/plugin.h"
#include "plugin/plugin_handler.h"

extern "C" void FuncForPlugin(rvsim::Plugin &plug, rvsim::PluginRegimes regime) 
{
    std::cout << "Plugin Id: " << std::int32_t(plug.GetID()) << std::endl;
    std::cout << "Plugin regime: " << std::int32_t(regime) << std::endl;

    switch (regime)
    {
    case rvsim::PluginRegimes::CoSimulation_run: /* initialization for cosimulation routine */
    {
        FILE *dumpFile = fopen("cosim.trace", "a");
        if (!dumpFile) {
            std::cerr << "can't open file for cosim\n";
            return;
        }

        std::cout << "Here1\n" << std::endl;

        rvsim::Instruction* cur_instr = plug.GetCurInstr();
        std::cout << "Here2\n" << std::endl;

        /* print insn and pc */
        rvsim::PluginHandler* handler = plug.GetHandler();
        fprintf(dumpFile, "PC:%li ", handler->GetPC());
        std::cout << "Here3:" << handler->GetPC() << "\n" << std::endl;
        fprintf(dumpFile, "INSTR_ID:%d \n", std::int32_t(cur_instr->id));
        std::cout << "Here4\n" << std::endl;

        /* print RegFile info */
        fprintf(dumpFile, "\t\tREGs: \n");

        fprintf(dumpFile, "rd:%li, ", handler->GetGPR(cur_instr->rd));
        fprintf(dumpFile, "rs1:%li, ", handler->GetGPR(cur_instr->rs1));
        fprintf(dumpFile, "rs2:%li, ", handler->GetGPR(cur_instr->rs2));
        fprintf(dumpFile, "rs3:%li, ", handler->GetGPR(cur_instr->rs3));
        fprintf(dumpFile, "imm:%li, ", handler->GetGPR(cur_instr->imm));
        fprintf(dumpFile, "rm:%li\n", handler->GetGPR(cur_instr->rm));

        fprintf(dumpFile, "#################################\n");

        //plug.getCurInsn()++;
        fclose(dumpFile);
        break;
    }
    default:
        std::cerr << "Undefinied regime of Plugin: " << std::int32_t(regime) << std::endl;
        break;
    }

}

//} //namespace rvsim