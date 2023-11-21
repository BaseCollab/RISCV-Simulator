#ifndef SIMULATOR_HART_HART_H
#define SIMULATOR_HART_HART_H

#include "instruction/instruction.h"
#include "common/constants.h"
#include "common/macros.h"
#include "common/config.h"
#include "mmu/mmu.h"
#include "hart/csr.h"
#include "hart/exception.h"
#include "hart/basic_block.h"

#include <functional>
#include <optional>
#include <cstdint>
#include <cassert>
#include <memory>
#include <elf.h>

//#include "plugin/plugin.h"
//#include "plugin/plugin_handler.h"

namespace rvsim {

class PluginHandler;

class Hart {
public:
    struct ExceptionHandlers {
        std::function<void(Exception, addr_t)> mmu_handler;
    };

public:
    NO_COPY_SEMANTIC(Hart);
    NO_MOVE_SEMANTIC(Hart);

    explicit Hart(PhysMemoryCtl *memory) : memory_(memory)
    {
        bb_manager_ = std::make_unique<BasicBlockManager>(this);
    }

    ~Hart() = default;

    Exception FetchInstruction(instr_size_t *raw_instr) const;

    void DecodeInstruction(Instruction *instr, instr_size_t raw_instr);

    void Interpret();
    void InterpretWithPlugins();

    void ExecuteBasicBlock(BasicBlock *bb);

    bool IsIdle() const
    {
        return is_idle_;
    }

    void SetIdleStatus(bool status)
    {
        is_idle_ = status;
    }

    const MMU &GetMMU() const
    {
        return mmu_;
    }

    void SetExceptionHandlers(const ExceptionHandlers &handlers)
    {
        handlers_ = handlers;
    }

    void SetPluginHandler(PluginHandler *plugin_handler)
    {
        plugin_handler_ = plugin_handler;
    }

    const PluginHandler *GetPluginHandler() const
    {
        return plugin_handler_;
    }

    reg_t GetPC() const
    {
        return pc_;
    }

    reg_t GetPCTarget() const
    {
        return pc_target_;
    }

    void SetPC(reg_t pc)
    {
        pc_ = pc;
    }

    void SetPCTarget(reg_t pc_target)
    {
        pc_target_ = pc_target;
    }

    gpr_t GetGPR(gpr_idx_t reg_idx) const
    {
        assert(reg_idx < N_GPR);
        return gpr_table_[reg_idx];
    }

    void SetGPR(gpr_idx_t reg_idx, gpr_t value)
    {
        assert(reg_idx < N_GPR);
        gpr_table_[reg_idx] = value;
    }

    Mode GetMode() const
    {
        return mode_;
    }

    // TODO: remove page-fault handling + rwx_flags argument
    template <typename ValueType>
    Exception LoadFromMemory(vaddr_t src, ValueType *value, uint8_t rwx_flags = PF_R) const
    {
        static_assert((sizeof(ValueType) == sizeof(byte_t)) || sizeof(ValueType) == sizeof(hword_t) ||
                      (sizeof(ValueType) == sizeof(word_t)) || sizeof(ValueType) == sizeof(dword_t));

        if ((src & (sizeof(ValueType) - 1)) != 0) {
            return Exception::MMU_ADDRESS_MISALIGNED;
        }

        paddr_t paddr {0};
        Exception exception {Exception::NONE};

        std::tie(paddr, exception) = mmu_.VirtToPhysAddr(src, rwx_flags, csr_regs, *memory_, mode_);
        if (exception != Exception::NONE) {
            return exception;
        }

        auto load_pair = memory_->Load<ValueType>(paddr);
        *value = load_pair.first;

        return Exception::NONE;
    }

    // TODO: remove page-fault handling + rwx_flags argument
    template <typename ValueType>
    Exception StoreToMemory(vaddr_t dst, ValueType value, uint8_t rwx_flags = PF_W) const
    {
        static_assert((sizeof(ValueType) == sizeof(byte_t)) || sizeof(ValueType) == sizeof(hword_t) ||
                      (sizeof(ValueType) == sizeof(word_t)) || sizeof(ValueType) == sizeof(dword_t));

        if ((dst & (sizeof(ValueType) - 1)) != 0) {
            return Exception::MMU_ADDRESS_MISALIGNED;
        }

        paddr_t paddr {0};
        Exception exception {Exception::NONE};

        std::tie(paddr, exception) = mmu_.VirtToPhysAddr(dst, rwx_flags, csr_regs, *memory_, mode_);
        if (exception != Exception::NONE) {
            return exception;
        }

        memory_->Store<ValueType>(paddr, value);

        return Exception::NONE;
    }

    Exception LoadFromMemory(void *dst, size_t dst_size, vaddr_t src, uint8_t rwx_flags = PF_R) const;
    Exception StoreToMemory(vaddr_t dst, void *src, size_t src_size, uint8_t rwx_flags = PF_W) const;

#ifdef DEBUG_HART
    template <typename T>
    void DumpRegs(T &out)
    {
        out << "[DEBUG] [REGS_DUMP: Start]" << std::endl;
        for (size_t i = 0; i < N_GPR; ++i) {
            out << "[DEBUG] [REGS_DUMP] X" << i << " = 0x" << std::hex << gpr_table_[i] << std::dec << std::endl;
        }
        out << "[DEBUG] [REGS_DUMP: End]" << std::endl;
    }
#endif

public:
    CSRs csr_regs;

private:
    Mode mode_ {Mode::USER_MODE};

    PhysMemoryCtl *memory_ {nullptr};
    MMU mmu_;

    gpr_t gpr_table_[N_GPR] = {0};

    reg_t pc_;
    reg_t pc_target_;

    ExceptionHandlers handlers_;
    PluginHandler *plugin_handler_;

    // idle is true, when hart does not follow instructions
    bool is_idle_ {true};

    std::unique_ptr<BasicBlockManager> bb_manager_;
};

} // namespace rvsim

#endif // SIMULATOR_HART_HART_H
