#include "plugin/plugin.h"
#include "plugin/plugin_handler.h"
#include "time.h"

extern "C" void FuncForPlugin(rvsim::Plugin &plug, rvsim::PluginRegimes regime) 
{

    static size_t instr_cnt = 0;
    static clock_t start = clock();

    switch (regime)
    {
        case rvsim::PluginRegimes::CoSimulation_run:
        {
            rvsim::PluginHandler* handler = plug.GetHandler();
            auto pc = handler->GetPC();
            if (pc != 65872) {
                ++instr_cnt;
            }
            else {
                clock_t end = clock();
                double elapsed_time = ((double) (end - start)) / CLOCKS_PER_SEC;
                std::cout << "Instr:" << instr_cnt << "\nTime: " << elapsed_time << std::endl;
                std::cout << "IPS:" << instr_cnt/elapsed_time << std::endl;
                std::cout << "MIPS:" << instr_cnt/elapsed_time/1000000 << std::endl;

                fprintf(stderr, "\t\tREGs: \n");

                for (size_t i = 0; i < 32; ++i) {
                    fprintf(stderr, "x%ld:%li\n", i, handler->GetGPR(i));
                }
                fprintf(stderr, "#################################\n");

            }
            break;
        }
    // case rvsim::PluginRegimes::CoSimulation_run: /* initialization for cosimulation routine */
    // {
    //     FILE *dumpFile = fopen("cosim.trace", "a");
    //     if (!dumpFile) {
    //         std::cerr << "can't open file for cosim\n";
    //         return;
    //     }

    //     const rvsim::Instruction* cur_instr = plug.GetCurInstr();

    //     /* print insn and pc */
    //     rvsim::PluginHandler* handler = plug.GetHandler();
    //     fprintf(dumpFile, "PC:%li ", handler->GetPC());
    //     std::cout << "Here3:" << handler->GetPC() << "\n" << std::endl;
    //     fprintf(dumpFile, "INSTR_ID:%d \n", std::int32_t(cur_instr->id));
    //     std::cout << "Here4\n" << std::endl;

    //     /* print RegFile info */
    //     fprintf(dumpFile, "\t\tREGs: \n");

    //     fprintf(dumpFile, "rd:%li, ", handler->GetGPR(cur_instr->rd));
    //     fprintf(dumpFile, "rs1:%li, ", handler->GetGPR(cur_instr->rs1));
    //     fprintf(dumpFile, "rs2:%li, ", handler->GetGPR(cur_instr->rs2));
    //     fprintf(dumpFile, "rs3:%li, ", handler->GetGPR(cur_instr->rs3));
    //     fprintf(dumpFile, "imm:%i, ", cur_instr->imm);
    //     fprintf(dumpFile, "rm:%li\n", handler->GetGPR(cur_instr->rm));

    //     fprintf(dumpFile, "#################################\n");

    //     //plug.getCurInsn()++;
    //     fclose(dumpFile);
    //     break;
    // }
    default:
        std::cerr << "Undefinied regime of Plugin: " << std::int32_t(regime) << std::endl;
        break;
    }

}

//} //namespace rvsim