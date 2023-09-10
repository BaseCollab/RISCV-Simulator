#ifndef SIMULATOR_MMU_MMU_H
#define SIMULATOR_MMU_MMU_H

#include "macros.h"
#include "constants.h"
#include "vpt.h"
#include "csr.h"
#include "memory_controller.h"

namespace rvsim {

class MMU {
public:
    enum class Exception
    {
        NONE                = -1,
        INVALID_PAGE_ENTRY  = 1,
        INVALID_PAGE_SIZE   = 2, // not all types of memory are supported (such as superpages)
        PAGE_WRITE_NO_READ  = 3,
        PAGE_ACCESS_READ    = 4,
        PAGE_ACCESS_WRITE   = 5,
        PAGE_ACCESS_EXECUTE = 6,
    };

    enum class Target
    {
        NONE    = -1,
        READ    = 1,
        WRITE   = 2,
        EXECUTE = 3,
    };

private:

    Exception ValidatePTE(const pte_t &pte, MMU::Target target) const;

    // TODO: TLB implementation

public:

    MMU() = default;
    ~MMU() = default;

    std::pair<paddr_t, Exception> VirtToPhysAddr(vaddr_t vaddr, Target target, const CSRs &csr_regs, MemoryCtl &memory);
};

} // rvsim

#endif // SIMULATOR_MMU_MMU_H
