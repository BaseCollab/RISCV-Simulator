#ifndef __PLUGIN_HANDLER__H_
#define __PLUGIN_HANDLER__H_

#include "hart/hart.h"

#include <dlfcn.h>
#include <stdlib.h>
#include <string>
#include <utility>
#include <vector>

namespace rvsim {
class Plugin;

class PluginHandler {
private:
    bool used_;
    std::vector<Plugin *> plugins_;
    const rvsim::Hart *hart_;

public:
    NO_COPY_SEMANTIC(PluginHandler);
    NO_MOVE_SEMANTIC(PluginHandler);
    explicit PluginHandler(std::vector<std::string> plugins, const rvsim::Hart *hart);
    ~PluginHandler();

    bool GetUsedStatus() const
    {
        return used_;
    }
    rvsim::reg_t GetPC() const
    {
        return hart_->GetPC();
    };
    Plugin *GetPlugin(size_t plug_idx)
    {
        return plugins_[plug_idx];
    };

    rvsim::reg_t GetGPR(gpr_idx_t reg_idx) const
    {
        return hart_->GetGPR(reg_idx);
    }

    std::vector<rvsim::reg_t> GetRegFile() const;

    void SetCurInstr(int plugin_id, const Instruction *instr);

    void CallPlugin(int plugin_id, rvsim::PluginRegimes regime) const;
};
} // namespace rvsim

#endif