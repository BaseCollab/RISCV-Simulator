#include "plugin_handler.h"
#include "common/macros.h"
#include "common/constants.h"
#include "common/config.h"
#include "hart/hart.h"

namespace rvsim {
    
    PluginHandler::PluginHandler(std::vector<std::string> plugins, const rvsim::Hart *hart) 
    {
        hart_ = hart;
        used_ = false;

        void *handle = nullptr;
        char *error = nullptr;
        PluginFunc_t plugEntry;

        int cur_id = 0;

        for (auto plug : plugins) 
        {
            handle = dlopen(plug.c_str(), RTLD_LAZY);
            if (!handle)
                fprintf(stderr, "Can't load plugin: %s\n", dlerror());

            dlerror();

            plugEntry = (PluginFunc_t)dlsym(handle, "FuncForPlugin");

            error = dlerror();

            if (error != NULL) {
                fprintf(stderr, "%s\n", error);
            }

            plugins_.push_back(new Plugin(cur_id++, handle, plugEntry, this));
        }

        used_ = (cur_id == 0);
    }

    PluginHandler::~PluginHandler() 
    {
        for (auto plug : plugins_) 
        {
            if (dlclose(plug->GetLib()) != 0) 
               fprintf(stderr, "Error on plugin close: %s\n", dlerror());

            delete plug;
        }
    }

    // rvsim::reg_t PluginHandler::GetGPR(gpr_idx_t reg_idx) const 
    // {
    //     return hart_->GetGPR(reg_idx);
    // }

    std::vector<rvsim::reg_t> PluginHandler::GetRegFile() const 
    {
        std::vector<rvsim::reg_t> regFile;
        for (size_t i = 0; i < rvsim::N_GPR - 1; i++) {
            regFile.push_back(hart_->GetGPR(i));
        }

        return regFile;
    }

    void PluginHandler::CallPlugin(int plugin_id, rvsim::PluginRegimes regime) const
    {
        plugins_[plugin_id]->CallPlugin(regime);
    }

    void PluginHandler::SetCurInstr(int plugin_id, Instruction *instr)
    {
        plugins_[plugin_id]->SetCurInstr(instr);
    }

}