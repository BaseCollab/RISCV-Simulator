#ifndef SIMULATOR_MMU_MMU_H
#define SIMULATOR_MMU_MMU_H

#include "common/macros.h"
#include "common/constants.h"
#include "supervisor/vpt.h"
#include "hart/csr.h"
#include "memory/memory_controller.h"

namespace rvsim {

class MMU {
public:
    // clang-format off
    enum class Exception {
        NONE                = -1,
        INVALID_PAGE_ENTRY  = 1,
        INVALID_PAGE_SIZE   = 2, // not all types of memory are supported (such as superpages)
        PAGE_WRITE_NO_READ  = 3,
        PAGE_ACCESS_READ    = 4,
        PAGE_ACCESS_WRITE   = 5,
        PAGE_ACCESS_EXECUTE = 6,
    };

    enum class Target {
        NONE    = -1,
        READ    = 1,
        WRITE   = 2,
        EXECUTE = 3,
    };
    // clang-format off

public:
    NO_COPY_SEMANTIC(MMU);
    NO_MOVE_SEMANTIC(MMU);

    MMU() = default;
    ~MMU() = default;

    std::pair<paddr_t, std::optional<MMU::Exception>> VirtToPhysAddr(vaddr_t vaddr, Target target, const CSRs &csr_regs, const PhysMemoryCtl &memory) const;

private:
    Exception ValidatePTE(const pte_t &pte, MMU::Target target) const;

    // TODO: TLB implementation
};

} // namespace rvsim

#endif // SIMULATOR_MMU_MMU_H
