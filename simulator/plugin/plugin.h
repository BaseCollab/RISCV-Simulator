#ifndef __PLUGIN__H_
#define __PLUGIN__H_

#include "common/macros.h"
#include "common/constants.h"
#include "common/config.h"
#include "memory/memory.h"

#include <inttypes.h>
#include <vector>

#include "hart/hart.h"
#include "plugin_handler.h"

namespace rvsim {

class Plugin;
// class PluginHandler;
using PluginFunc_t = void (*)(Plugin &, PluginRegimes);

class Plugin {
private:
    friend class PluginHandler;

    /* unique id of a plugin */
    int id_;

    /* cur instruction (in BB) */
    const Instruction *cur_instr_;

    /* class that handle all plugin */
    PluginHandler *handler_;

    /* function to execute given functionality */
    PluginFunc_t plugin_func_;

    /* shared library */
    void *load_lib_;

    /* regime of plugin (specified in enum) */
    rvsim::PluginRegimes regime_;

private:
    PluginFunc_t GetPluginFunc() const
    {
        return plugin_func_;
    }

    void *GetLib() const
    {
        return load_lib_;
    }

public:
    int GetID() const
    {
        return id_;
    }

public:
    NO_COPY_SEMANTIC(Plugin);
    NO_MOVE_SEMANTIC(Plugin);
    Plugin(int id, void *load_lib, PluginFunc_t plugin_func, rvsim::PluginHandler *handler)
        : id_(id), cur_instr_(nullptr), handler_(handler), plugin_func_(plugin_func), load_lib_(load_lib) {};

    void SetPluginRegime(rvsim::PluginRegimes regime)
    {
        regime_ = regime;
    };
    rvsim::PluginRegimes GetPluginRegime()
    {
        return regime_;
    };

    void CallPlugin(rvsim::PluginRegimes regime)
    {
        plugin_func_(*this, regime);
    };

    void SetCurInstr(const Instruction *instr)
    {
        cur_instr_ = instr;
    }
    const Instruction *GetCurInstr()
    {
        return cur_instr_;
    };

    PluginHandler *GetHandler()
    {
        return handler_;
    }

    rvsim::reg_t GetPC() const; // { return handler_->GetPC(); };
    rvsim::reg_t GetGPR(gpr_idx_t reg_idx) const;
    std::vector<rvsim::reg_t> GetRegFile() const; // { return handler_->GetRegFile(); };
};

} // namespace rvsim

#endif