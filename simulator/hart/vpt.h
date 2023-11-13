#ifndef SIMULATOR_VPT_VPT_H
#define SIMULATOR_VPT_VPT_H

#include "common/utils/bit_ops.h"
#include "common/macros.h"
#include "common/constants.h"
#include "common/config.h"

#include <cstdint>

namespace rvsim {

// clang-format off

// Sv48 page table entry (PTE)
struct pte_t {
    dword_t value;

    static constexpr dword_t mask_n     = bitops::Ones<63, 63>(); // not implemented yet
    static constexpr dword_t mask_pbmt  = bitops::Ones<62, 61>(); // not implemented yet
    static constexpr dword_t mask_res_1 = bitops::Ones<60, 54>();
    static constexpr dword_t mask_ppn   = bitops::Ones<53, 10>(); // PPN = Physical page number
    static constexpr dword_t mask_rsw   = bitops::Ones<9, 8>(); // is reserved for use by supervisor software
    static constexpr dword_t mask_d     = bitops::Ones<7, 7>(); // dirty: not implemented yet
    static constexpr dword_t mask_a     = bitops::Ones<6, 6>(); // accessed: not implemented yet
    static constexpr dword_t mask_g     = bitops::Ones<5, 5>(); // global mapping: not implemented yet
    static constexpr dword_t mask_u     = bitops::Ones<4, 4>(); // page is accessible to user mode: not implemented yet
    static constexpr dword_t mask_x     = bitops::Ones<3, 3>(); // executable
    static constexpr dword_t mask_w     = bitops::Ones<2, 2>(); // write
    static constexpr dword_t mask_r     = bitops::Ones<1, 1>(); // read
    static constexpr dword_t mask_v     = bitops::Ones<0, 0>(); // whether the PTE is valid

    pte_t():            value(0)   {}
    pte_t(dword_t val): value(val) {}

    dword_t GetPPN() const { return (value & mask_ppn) >> 10; }
    void SetPPN(dword_t ppn) { value = (value & ~mask_ppn) | (ppn << 10); }

    dword_t GetU() const { return (value & mask_u) >> 4; }
    void SetU(dword_t u) { value = (value & ~mask_u) | (u << 4); }

    dword_t GetX() const { return (value & mask_x) >> 3; }
    void SetX(dword_t x) { value = (value & ~mask_x) | (x << 3); }

    dword_t GetW() const { return (value & mask_w) >> 2; }
    void SetW(dword_t w) { value = (value & ~mask_w) | (w << 2); }

    dword_t GetR() const { return (value & mask_r) >> 1; }
    void SetR(dword_t r) { value = (value & ~mask_r) | (r << 1); }

    dword_t GetV() const { return (value & mask_v); }
    void SetV(dword_t v) { value = (value & ~mask_v) | v; }
};

struct vpt_t {
    pte_t vpt[1 << 9]; // sizeof_bits(vaddr_t(ppn_3))
};

struct paddr_t {
    addr_t value;

    static constexpr dword_t mask_ppn         = bitops::Ones<55, 12>();
    static constexpr dword_t mask_page_offset = bitops::Ones<11, 0>();

    static constexpr size_t n_offset_bits     = 12;

    paddr_t():           value(0)   {}
    paddr_t(addr_t val): value(val) {}
    paddr_t(const paddr_t &paddr)  { value = paddr.value; }

    addr_t operator=(const addr_t &val) { value = val; return value;}
    paddr_t &operator=(const paddr_t &paddr) { value = paddr.value; return *this;}
    operator addr_t() const { return value; }

    dword_t GetPPN() const { return (value & mask_ppn) >> 12; }
    void SetPPN(dword_t ppn) { value = (value & ~mask_ppn) | (ppn << 12); }

    dword_t GetPageOffset() const { return (value & mask_page_offset); }
    void SetPageOffset(dword_t offset) { value = (value & ~mask_page_offset) | offset; }
};

struct vaddr_t {
    addr_t value;

    static constexpr dword_t mask_vpn_3       = bitops::Ones<47, 39>();
    static constexpr dword_t mask_vpn_2       = bitops::Ones<38, 30>();
    static constexpr dword_t mask_vpn_1       = bitops::Ones<29, 21>();
    static constexpr dword_t mask_vpn_0       = bitops::Ones<20, 12>();
    static constexpr dword_t mask_page_offset = bitops::Ones<11, 0>();

    static constexpr size_t n_offset_bits     = 12;

    vaddr_t():           value(0)   {}
    vaddr_t(addr_t val): value(val) {}
    vaddr_t(const vaddr_t &vaddr)  { value = vaddr.value; }

    addr_t operator=(const addr_t &val) { value = val; return value; }
    vaddr_t &operator=(const vaddr_t &vaddr) { value = vaddr.value; return *this; }
    operator addr_t() const { return value; }

    dword_t GetVPN0() const { return (value & mask_vpn_0) >> 12; }
    void SetVPN0(dword_t ppn) { value = (value & ~mask_vpn_0) | (ppn << 12); }

    dword_t GetVPN1() const { return (value & mask_vpn_1) >> 21; }
    void SetVPN1(dword_t ppn) { value = (value & ~mask_vpn_1) | (ppn << 21); }

    dword_t GetVPN2() const { return (value & mask_vpn_2) >> 30; }
    void SetVPN2(dword_t ppn) { value = (value & ~mask_vpn_2) | (ppn << 30); }

    dword_t GetVPN3() const { return (value & mask_vpn_3) >> 39; }
    void SetVPN3(dword_t ppn) { value = (value & ~mask_vpn_3) | (ppn << 39); }

    dword_t GetPageOffset() const { return (value & mask_page_offset); }
    void SetPageOffset(dword_t offset) { value = (value & ~mask_page_offset) | offset; }
};

static_assert(sizeof(paddr_t) == 8, "sizeof(paddr_t) != sizeof(addr_t)");
static_assert(sizeof(vaddr_t) == 8, "sizeof(vaddr_t) != sizeof(addr_t)");
static_assert(sizeof(pte_t)   == 8, "sizeof(pte_t) != sizeof(uint64_t)");
// clang-format on

} // namespace rvsim

#endif // SIMULATOR_VPT_VPT_H
