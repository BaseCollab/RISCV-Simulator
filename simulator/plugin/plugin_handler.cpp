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
    Plugin::func_t plug_entry;

    int cur_id = 0;

    for (auto plug : plugins) {
        handle = dlopen(plug.c_str(), RTLD_LAZY);
        if (!handle)
            fprintf(stderr, "Can't load plugin: %s\n", dlerror());

        dlerror();

        plug_entry = (Plugin::func_t)dlsym(handle, "FuncForPlugin");

        error = dlerror();

        if (error != NULL) {
            fprintf(stderr, "%s\n", error);
        }

        plugins_.push_back(new Plugin(cur_id++, handle, plug_entry, this));
    }

    used_ = (cur_id == 0);
}

PluginHandler::~PluginHandler()
{
    for (auto plug : plugins_) {
        if (dlclose(plug->GetLib()) != 0)
            fprintf(stderr, "Error on plugin close: %s\n", dlerror());

        delete plug;
    }
}

std::vector<rvsim::reg_t> PluginHandler::GetRegFile() const
{
    std::vector<rvsim::reg_t> reg_file;
    for (size_t i = 0; i < rvsim::N_GPR - 1; i++) {
        reg_file.push_back(hart_->GetGPR(i));
    }

    return reg_file;
}

void PluginHandler::CallPlugin(int plugin_id, PluginRegimes regime) const
{
    plugins_[plugin_id]->CallPlugin(regime);
}

void PluginHandler::SetCurInstr(int plugin_id, const Instruction *instr)
{
    plugins_[plugin_id]->SetCurInstr(instr);
}

} // namespace rvsim