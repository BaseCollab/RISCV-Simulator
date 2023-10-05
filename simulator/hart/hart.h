#ifndef SIMULATOR_HART_HART_H
#define SIMULATOR_HART_HART_H

#include "common/constants.h"
#include "common/macros.h"
#include "common/config.h"
#include "mmu/mmu.h"
#include "csr.h"

#include <cstdint>
#include <elf.h>

namespace rvsim {

class Hart {
public:
    struct ExceptionHandlers {
        std::function<void(MMU::Exception)> mmu_handler;
    };

public:
    NO_COPY_SEMANTIC(Hart);
    NO_MOVE_SEMANTIC(Hart);

    explicit Hart(PhysMemoryCtl *memory) : memory_(memory) {};
    ~Hart() = default;

    void SetExceptionHandlers(const ExceptionHandlers &handlers);

    const MMU &GetMMU() const
    {
        return mmu_;
    }

    reg_t GetPC() const;
    void SetPC(reg_t pc);

    reg_t GetPCTarget() const;
    void SetPCTarget(reg_t pc_target);

    gpr_t GetGPR(gpr_idx_t reg_idx) const;
    void SetGPR(gpr_idx_t reg_idx, gpr_t value);

    void LoadFromMemory(void *dst, size_t dst_size, vaddr_t src, uint8_t rwx_flags = 0 | PF_R | PF_W) const;
    void StoreToMemory(vaddr_t dst, void *src, size_t src_size, uint8_t rwx_flags = 0 | PF_R | PF_W) const;

    template <typename ValueType>
    reg_t LoadFromMemory(vaddr_t src, uint8_t rwx_flags = 0 | PF_R | PF_W) const;

    template <typename ValueType>
    void StoreToMemory(vaddr_t dst, reg_t value, uint8_t rwx_flags = 0 | PF_R | PF_W) const;

public:
    CSRs csr_regs;

private:
    PhysMemoryCtl *memory_ {nullptr};
    MMU mmu_;

    gpr_t gpr_table_[N_GPR];

    reg_t pc_;
    reg_t pc_target_;

    ExceptionHandlers handlers_;
};

} // namespace rvsim

#endif // SIMULATOR_HART_HART_H
