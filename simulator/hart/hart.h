#ifndef SIMULATOR_HART_HART_H
#define SIMULATOR_HART_HART_H

#include "instruction/instruction.h"
#include "common/constants.h"
#include "common/macros.h"
#include "common/config.h"
#include "mmu/mmu.h"
#include "csr.h"

#include <functional>
#include <cstdint>
#include <cassert>
#include <elf.h>

namespace rvsim {

class Hart {
public:
    struct ExceptionHandlers {
        std::function<void(MMU::Exception, addr_t, uint8_t)> mmu_handler;
    };

public:
    NO_COPY_SEMANTIC(Hart);
    NO_MOVE_SEMANTIC(Hart);

    explicit Hart(PhysMemoryCtl *memory) : memory_(memory) {};
    ~Hart() = default;

    instr_size_t FetchInstruction();

    void Interpret();

    void DecodeAndExecute(Instruction *instr, instr_size_t raw_instr);

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

    template <typename ValueType>
    ValueType LoadFromMemory(vaddr_t src, uint8_t rwx_flags = 0 | PF_R | PF_W) const
    {
        auto pair_paddr = mmu_.VirtToPhysAddr(src, rwx_flags, csr_regs, *memory_);
        if (pair_paddr.second != MMU::Exception::NONE) {
            handlers_.mmu_handler(pair_paddr.second, src, rwx_flags);
            pair_paddr = mmu_.VirtToPhysAddr(src, rwx_flags, csr_regs, *memory_);
        }

        auto load_pair = memory_->Load<ValueType>(pair_paddr.first.value);
        return load_pair.first;
    }

    template <typename ValueType>
    void StoreToMemory(vaddr_t dst, ValueType value, uint8_t rwx_flags = 0 | PF_R | PF_W) const
    {
        auto pair_paddr = mmu_.VirtToPhysAddr(dst, rwx_flags, csr_regs, *memory_);
        if (pair_paddr.second != MMU::Exception::NONE) {
            handlers_.mmu_handler(pair_paddr.second, dst, rwx_flags);
            pair_paddr = mmu_.VirtToPhysAddr(dst, rwx_flags, csr_regs, *memory_);
        }

        memory_->Store<ValueType>(pair_paddr.first.value, value);
    }

    void LoadFromMemory(void *dst, size_t dst_size, vaddr_t src, uint8_t rwx_flags = 0 | PF_R | PF_W) const;
    void StoreToMemory(vaddr_t dst, void *src, size_t src_size, uint8_t rwx_flags = 0 | PF_R | PF_W) const;

public:
    CSRs csr_regs;

private:
    PhysMemoryCtl *memory_ {nullptr};
    MMU mmu_;

    gpr_t gpr_table_[N_GPR] = {0};

    reg_t pc_;
    reg_t pc_target_;

    ExceptionHandlers handlers_;

    // idle is true, when hart does not follow instructions
    bool is_idle_ {true};
};

} // namespace rvsim

#endif // SIMULATOR_HART_HART_H
