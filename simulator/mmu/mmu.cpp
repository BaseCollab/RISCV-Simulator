#include "mmu.h"

#include <bitset>
#include <optional>
#include <utility>
#include <cstring>
#include <elf.h>

namespace rvsim {

template <bool IsLastLevel>
Exception MMU::ValidatePTE(const pte_t &pte, const CSRs &csr_regs, Mode mode, uint8_t rwx_flags) const
{
    if (pte.GetV() == 0) // not valid page table entry
        return Exception::MMU_INVALID_PAGE_ENTRY;

    if constexpr (IsLastLevel == true) {
        auto mstatus = csr_regs.LoadCSR<CSRs::mstatus_t>(CSRs::Index::MSTATUS);

        if (pte.GetW() == 1 && pte.GetR() == 0)
            return Exception::MMU_PAGE_WRITE_NO_READ;

        if (mstatus.fields.mxr == 1) {
            if ((rwx_flags & PF_R) && (pte.GetR() == 0) && (pte.GetX() == 0))
                return Exception::MMU_PAGE_ACCESS_READ;
        } else if ((rwx_flags & PF_R) && (pte.GetR() == 0)) {
            return Exception::MMU_PAGE_ACCESS_READ;
        }

        if (mstatus.fields.sum == 0) {
            if ((mode == Mode::SUPERVISOR_MODE) && pte.GetU() == 1) {
                return Exception::MMU_PAGE_SUPERVISOR_USER_ACCESS;
            }
        }

        if (pte.GetW() == 1 && pte.GetR() == 0)
            return Exception::MMU_PAGE_WRITE_NO_READ;

        if ((rwx_flags & PF_W) && (pte.GetW() == 0))
            return Exception::MMU_PAGE_ACCESS_WRITE;

        if ((rwx_flags & PF_X) && (pte.GetX() == 0))
            return Exception::MMU_PAGE_ACCESS_EXECUTE;
    }
    // TODO: add all other possible exceptions

    return Exception::NONE;
}

// TODO: update A and D flags in final PTE
std::pair<paddr_t, Exception> MMU::VirtToPhysAddr(vaddr_t vaddr, uint8_t rwx_flags, const CSRs &csr_regs,
                                                  const PhysMemoryCtl &memory, Mode mode) const
{
    paddr_t paddr = 0;

    pte_t pte_3;
    pte_t pte_2;
    pte_t pte_1;
    pte_t pte_0;

    Exception exception;

    auto satp = csr_regs.LoadCSR<CSRs::satp_t>(CSRs::Index::SATP);

    memory.Load(&pte_3, sizeof(pte_3), satp.fields.ppn * VPAGE_SIZE + vaddr.GetVPN3() * sizeof(pte_t));

#ifdef DEBUG_MMU
    std::bitset<bitops::BitSizeof<vaddr_t>()> vaddr_bitset(vaddr.value);
    std::cerr << "[DEBUG] [MMU: Start]" << std::endl;
    std::cerr << "[DEBUG] [MMU] vaddr = " << vaddr_bitset << std::endl;
    std::cerr << "[DEBUG] [MMU] vaddr = 0x" << std::hex << vaddr.value << std::endl;
    std::cerr << "[DEBUG] [MMU] vaddr.offset = 0x" << vaddr.GetPageOffset() << std::dec << std::endl;

    std::cerr << "[DEBUG] [MMU] 1) satp.ppn = 0x" << std::hex << satp.fields.ppn << std::endl;
    std::cerr << "[DEBUG] [MMU]    vaddr.vpn3 = 0x" << vaddr.GetVPN3() << std::endl;
    std::cerr << "[DEBUG] [MMU]    pte_3_entry_addr = 0x" << satp.fields.ppn * VPAGE_SIZE + vaddr.GetVPN3() << std::dec << std::endl;
#endif

    if (pte_3.GetX() == 0 && pte_3.GetW() == 0 && pte_3.GetR() == 0) // pointer to next level
    {
        exception = ValidatePTE<false>(pte_3, csr_regs, mode);
        if (exception != Exception::NONE)
            return std::make_pair(paddr, exception);

        memory.Load(&pte_2, sizeof(pte_2), pte_3.GetPPN() * VPAGE_SIZE + vaddr.GetVPN2() * sizeof(pte_t));

#ifdef DEBUG_MMU
        std::cerr << "[DEBUG] [MMU] 2) pte_3.ppn = 0x" << std::hex << pte_3.GetPPN() << std::endl;
        std::cerr << "[DEBUG] [MMU]    vaddr.vpn2 = 0x" << vaddr.GetVPN2() << std::endl;
        std::cerr << "[DEBUG] [MMU]    pte_2_entry_addr = 0x" << pte_3.GetPPN() * VPAGE_SIZE + vaddr.GetVPN2() << std::dec << std::endl;
#endif

        if (pte_2.GetX() == 0 && pte_2.GetW() == 0 && pte_2.GetR() == 0) // pointer to next level
        {
            exception = ValidatePTE<false>(pte_2, csr_regs, mode);
            if (exception != Exception::NONE)
                return std::make_pair(paddr, exception);

            memory.Load(&pte_1, sizeof(pte_1), pte_2.GetPPN() * VPAGE_SIZE + vaddr.GetVPN1() * sizeof(pte_t));

#ifdef DEBUG_MMU
            std::cerr << "[DEBUG] [MMU] 3) pte_2.ppn = 0x" << std::hex << pte_2.GetPPN() << std::endl;
            std::cerr << "[DEBUG] [MMU]    vaddr.vpn1 = 0x" << vaddr.GetVPN1() << std::endl;
            std::cerr << "[DEBUG] [MMU]    pte_1_entry_addr = 0x" << pte_2.GetPPN() * VPAGE_SIZE + vaddr.GetVPN1() << std::dec
                      << std::endl;
#endif

            if (pte_1.GetX() == 0 && pte_1.GetW() == 0 && pte_1.GetR() == 0) // pointer to next level
            {
                exception = ValidatePTE<false>(pte_1, csr_regs, mode);
                if (exception != Exception::NONE)
                    return std::make_pair(paddr, exception);

                memory.Load(&pte_0, sizeof(pte_0), pte_1.GetPPN() * VPAGE_SIZE + vaddr.GetVPN0() * sizeof(pte_t));

#ifdef DEBUG_MMU
                std::cerr << "[DEBUG] [MMU] 4) pte_1.ppn = 0x" << std::hex << pte_1.GetPPN() << std::endl;
                std::cerr << "[DEBUG] [MMU]    vaddr.vpn0 = 0x" << vaddr.GetVPN0() << std::endl;
                std::cerr << "[DEBUG] [MMU]    pte_0_entry_addr = 0x" << pte_1.GetPPN() * VPAGE_SIZE + vaddr.GetVPN0() << std::dec
                          << std::endl;
#endif

                exception = ValidatePTE<true>(pte_0, csr_regs, mode, rwx_flags);

#ifdef DEBUG_MMU
                std::cerr << "[DEBUG] [MMU] 5) pte_0_entry->ppn exception: " << (int)exception << std::endl;
#endif

                if (exception != Exception::NONE)
                    return std::make_pair(paddr, exception);

                paddr.SetPageOffset(vaddr.GetPageOffset());
                paddr.SetPPN(pte_0.GetPPN());

#ifdef DEBUG_MMU
                std::bitset<bitops::BitSizeof<paddr_t>()> paddr_bitset(paddr.value);
                std::cerr << "[DEBUG] [MMU] paddr = 0b" << paddr_bitset << std::endl;
                std::cerr << "[DEBUG] [MMU] paddr = 0x" << std::hex << paddr.value << std::dec << std::endl;
                std::cerr << "[DEBUG] [MMU: End]" << std::endl;
#endif

            } else // TODO: support other types of pages
                return std::make_pair(paddr, Exception::MMU_INVALID_PAGE_SIZE);
        } else // TODO: support other types of pages
            return std::make_pair(paddr, Exception::MMU_INVALID_PAGE_SIZE);
    } else // TODO: support other types of pages
        return std::make_pair(paddr, Exception::MMU_INVALID_PAGE_SIZE);

    return std::make_pair(paddr, Exception::NONE);
}

} // namespace rvsim
