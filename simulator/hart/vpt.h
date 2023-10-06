#ifndef SIMULATOR_VPT_VPT_H
#define SIMULATOR_VPT_VPT_H

#include "common/macros.h"
#include "common/constants.h"
#include "common/config.h"

#include <cstdint>

namespace rvsim {

// clang-format off
struct pte_t { // Sv48 page table entry (PTE)
    uint64_t n          : 1 = 0; // not implemented yet
    uint64_t pbmt       : 2 = 0; // not implemented yet

    uint64_t reserved_1 : 7 = 0;

    // PPN = Physical page number
    uint64_t ppn        : 44 = 0;

    // uint64_t ppn_3      : 17;
    // uint64_t ppn_2      : 9;
    // uint64_t ppn_1      : 9;
    // uint64_t ppn_0      : 9;

    uint64_t rsw        : 2 = 0; // is reserved for use by supervisor software

    uint64_t d          : 1 = 0; // dirty: not implemented yet
    uint64_t a          : 1 = 0; // accessed: not implemented yet
    uint64_t g          : 1 = 0; // global mapping: not implemented yet
    uint64_t u          : 1 = 0; // page is accessible to user mode: not implemented yet
    uint64_t x          : 1 = 0; // executable
    uint64_t w          : 1 = 0; // write
    uint64_t r          : 1 = 0; // read
    uint64_t v          : 1 = 0; // whether the PTE is valid
};

union paddr_t {
    paddr_t(addr_t val): value(val) {}
    struct { // Sv48 page table entry (PTE)
        addr_t padding     : 8; // 64 - 56

        // PPN = Physical page number
        addr_t ppn         : 44;

        // addr_t ppn_3       : 17;
        // addr_t ppn_2       : 9;
        // addr_t ppn_1       : 9;
        // addr_t ppn_0       : 9;

        addr_t page_offset : 12;
    } fields;

    addr_t value;
};

union vaddr_t {
    vaddr_t(addr_t val): value(val) {}
    struct { // Sv48 page table entry (PTE)
        addr_t padding     : 16; // 64 - 48

        // PPN = Physical page number
        addr_t vpn_3       : 9;
        addr_t vpn_2       : 9;
        addr_t vpn_1       : 9;
        addr_t vpn_0       : 9;

        addr_t page_offset : 12;
    } fields;

    addr_t value;
};

struct vpt_t {
    pte_t vpt[1 << 9]; // sizeof_bits(vaddr_t(ppn_3))
};

static_assert(sizeof(paddr_t) == 8, "sizeof(paddr_t) != sizeof(addr_t)");
static_assert(sizeof(vaddr_t) == 8, "sizeof(vaddr_t) != sizeof(addr_t)");
static_assert(sizeof(pte_t)   == 8, "sizeof(pte_t) != sizeof(uint64_t)");
// clang-format on

} // namespace rvsim

#endif // SIMULATOR_VPT_VPT_H