#ifndef SIMULATOR_HART_HART_H
#define SIMULATOR_HART_HART_H

#include "instruction/instruction.h"
#include "common/constants.h"
#include "common/macros.h"
#include "common/config.h"
#include "mmu/mmu.h"
#include "mmu/tlb.h"
#include "hart/csr.h"
#include "hart/exception.h"
#include "hart/basic_block_manager.h"

#include <functional>
#include <optional>
#include <cstdint>
#include <cassert>
#include <memory>
#include <elf.h>
#include <gtest/gtest_prod.h>

namespace rvsim {

class PluginHandler;

class Hart {
public:
    struct ExceptionHandlers {
        std::function<void(Exception, addr_t)> default_handler;
    };

    using TLB_t = TLB<TLB_SIZE_LOG_2>;

public:
    NO_COPY_SEMANTIC(Hart);
    NO_MOVE_SEMANTIC(Hart);

    explicit Hart(PhysMemoryCtl *memory) : memory_(memory), mmu_(this)
    {
        bb_manager_ = std::make_unique<BasicBlockManager>(this);
    }

    ~Hart() = default;

    Exception FetchInstruction(instr_size_t *raw_instr);

    void DecodeInstruction(Instruction *instr, instr_size_t raw_instr);

    Exception Interpret();
    Exception InterpretWithPlugins();

    Exception ExecuteBasicBlock(const BasicBlock &bb);
    Exception ExecuteBasicBlockWithPlugins(const BasicBlock &bb);

    void SetPluginHandler(PluginHandler *plugin_handler)
    {
        plugin_handler_ = plugin_handler;
    }

    const PluginHandler *GetPluginHandler() const
    {
        return plugin_handler_;
    }

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

    template <typename ValueType>
    Exception LoadFromMemory(vaddr_t src, ValueType *value, uint8_t rwx_flags = PF_R)
    {
        static_assert((sizeof(ValueType) == sizeof(byte_t)) || sizeof(ValueType) == sizeof(hword_t) ||
                      (sizeof(ValueType) == sizeof(word_t)) || sizeof(ValueType) == sizeof(dword_t));

        if ((src & (sizeof(ValueType) - 1)) != 0) {
            return Exception::MMU_ADDRESS_MISALIGNED;
        }

        byte_t *host_addr {nullptr};
        Exception exception {Exception::NONE};

        vaddr_t src_page_addr = src & ~vaddr_t::mask_page_offset;

        const TLB_t::Data *cached_addr = rtlb_.LookUp(src_page_addr);
        if (cached_addr == nullptr) {
            paddr_t paddr {0};
            std::tie(paddr, exception) = mmu_.VirtToPhysAddr(src, rwx_flags, csr_regs, *memory_);
            if (exception != Exception::NONE) {
                return exception;
            }

            paddr_t tlb_paddr = (paddr & ~paddr_t::mask_page_offset);
            rtlb_.Update(TLB_t::Data {.host_addr = (memory_->GetRAM() + tlb_paddr), .paddr = tlb_paddr}, src_page_addr);

            host_addr = memory_->GetRAM() + tlb_paddr + (src & vaddr_t::mask_page_offset);
        } else {
            host_addr = cached_addr->host_addr + (src & vaddr_t::mask_page_offset);
        }

        std::memcpy(value, host_addr, sizeof(ValueType));

        return Exception::NONE;
    }

    template <typename ValueType>
    Exception StoreToMemory(vaddr_t dst, ValueType value, uint8_t rwx_flags = PF_W)
    {
        static_assert((sizeof(ValueType) == sizeof(byte_t)) || sizeof(ValueType) == sizeof(hword_t) ||
                      (sizeof(ValueType) == sizeof(word_t)) || sizeof(ValueType) == sizeof(dword_t));

        if ((dst & (sizeof(ValueType) - 1)) != 0) {
            return Exception::MMU_ADDRESS_MISALIGNED;
        }

        byte_t *host_addr {nullptr};
        Exception exception {Exception::NONE};

        vaddr_t dst_page_addr = dst & ~vaddr_t::mask_page_offset;

        const TLB_t::Data *cached_addr = wtlb_.LookUp(dst_page_addr);
        if (cached_addr == nullptr) {
            paddr_t paddr {0};
            std::tie(paddr, exception) = mmu_.VirtToPhysAddr(dst, rwx_flags, csr_regs, *memory_);
            if (exception != Exception::NONE) {
                return exception;
            }

            paddr_t tlb_paddr = (paddr & ~paddr_t::mask_page_offset);
            wtlb_.Update(TLB_t::Data {.host_addr = (memory_->GetRAM() + tlb_paddr), .paddr = tlb_paddr}, dst_page_addr);

            host_addr = memory_->GetRAM() + tlb_paddr + (dst & vaddr_t::mask_page_offset);
        } else {
            host_addr = cached_addr->host_addr + (dst & vaddr_t::mask_page_offset);
        }

        std::memcpy(host_addr, &value, sizeof(ValueType));

        return Exception::NONE;
    }

    Exception LoadFromMemory(void *dst, size_t dst_size, vaddr_t src, uint8_t rwx_flags = PF_R);
    Exception StoreToMemory(vaddr_t dst, void *src, size_t src_size, uint8_t rwx_flags = PF_W);

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
    TLB_t itlb_;
    TLB_t rtlb_;
    TLB_t wtlb_;

    gpr_t gpr_table_[N_GPR] = {};

    reg_t pc_;
    reg_t pc_target_;

    ExceptionHandlers handlers_;
    PluginHandler *plugin_handler_;

    // idle is true, when hart does not follow instructions
    bool is_idle_ {true};

    std::unique_ptr<BasicBlockManager> bb_manager_;

private:
    FRIEND_TEST(MMUTest, ComplexTest);
};

} // namespace rvsim

#endif // SIMULATOR_HART_HART_H
