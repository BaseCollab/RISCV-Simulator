#ifndef SIMULATOR_MMU_MMU_H
#define SIMULATOR_MMU_MMU_H

#include "common/macros.h"
#include "common/constants.h"
#include "hart/vpt.h"
#include "hart/csr.h"
#include "hart/exception.h"
#include "memory/memory_controller.h"
#include "mmu/tlb.h"

namespace rvsim {

class Hart;

class MMU {
public:
    NO_COPY_SEMANTIC(MMU);
    NO_MOVE_SEMANTIC(MMU);

    MMU(Hart *hart) : hart_(hart) {}
    ~MMU() = default;

    std::pair<paddr_t, Exception> VirtToPhysAddr(vaddr_t vaddr, uint8_t rwx_flags, const CSRs &csr_regs,
                                                 const PhysMemoryCtl &memory) const;

private:
    template <bool IsLastLevel>
    Exception ValidatePTE(const pte_t &pte, const CSRs &csr_regs, Mode mode, uint8_t rwx_flags = 0) const;

private:
    Hart *hart_ {nullptr};
};

} // namespace rvsim

#endif // SIMULATOR_MMU_MMU_H
