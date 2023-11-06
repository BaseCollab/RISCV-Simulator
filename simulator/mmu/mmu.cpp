#include "mmu.h"

#include <bitset>
#include <optional>
#include <utility>
#include <cstring>
#include <elf.h>

namespace rvsim {

MMU::Exception MMU::ValidatePTE(const pte_t &pte, uint8_t rwx_flags) const
{
    if (pte.GetV() == 0) // not valid page table entry
        return MMU::Exception::INVALID_PAGE_ENTRY;

    if (pte.GetW() == 1 && pte.GetR() == 0)
        return MMU::Exception::PAGE_WRITE_NO_READ;

    if ((rwx_flags & PF_R) && (pte.GetR() == 0))
        return MMU::Exception::PAGE_ACCESS_READ;

    if ((rwx_flags & PF_W) && (pte.GetW() == 0))
        return MMU::Exception::PAGE_ACCESS_WRITE;

    if ((rwx_flags & PF_X) && (pte.GetX() == 0))
        return MMU::Exception::PAGE_ACCESS_EXECUTE;

    // TODO: add all other possible exceptions

    return MMU::Exception::NONE;
}

std::pair<paddr_t, MMU::Exception> MMU::VirtToPhysAddr(vaddr_t vaddr, uint8_t rwx_flags, const CSRs &csr_regs,
                                                       const PhysMemoryCtl &memory) const
{
    paddr_t paddr = 0;

    pte_t pte_3;
    pte_t pte_2;
    pte_t pte_1;
    pte_t pte_0;

    MMU::Exception exception;

    csr_t satp_reg = csr_regs.LoadCSR(CSR_SATP_IDX);
    csr_satp_t satp;
    std::memcpy(&satp, &satp_reg, sizeof(satp_reg));

    memory.Load(&pte_3, sizeof(pte_3), satp.ppn * VPAGE_SIZE + vaddr.GetVPN3());

#ifdef DEBUG_MMU
    std::bitset<bitops::BitSizeof<vaddr_t>()> vaddr_bitset(vaddr.value);
    std::cerr << "[DEBUG] [MMU: Start]" << std::endl;
    std::cerr << "[DEBUG] [MMU] vaddr = " << vaddr_bitset << std::endl;
    std::cerr << "[DEBUG] [MMU] vaddr = 0x" << std::hex << vaddr.value << std::dec << std::endl;
    std::cerr << "[DEBUG] [MMU] vaddr.offset = " << vaddr.GetPageOffset() << std::endl;

    std::cerr << "[DEBUG] [MMU] satp.ppn = " << satp.ppn << std::endl;
    std::cerr << "[DEBUG] [MMU] vaddr.vpn3 = " << vaddr.GetVPN3() << std::endl;
    std::cerr << "[DEBUG] [MMU] pte_3_entry_addr = " << satp.ppn * VPAGE_SIZE + vaddr.GetVPN3() << std::endl;
#endif

    if (pte_3.GetX() == 0 && pte_3.GetW() == 0 && pte_3.GetR() == 0) // pointer to next level
    {
        exception = ValidatePTE(pte_3, 0);
        if (exception != MMU::Exception::NONE)
            return std::make_pair(paddr, exception);

        memory.Load(&pte_2, sizeof(pte_2), pte_3.GetPPN() * VPAGE_SIZE + vaddr.GetVPN2());

#ifdef DEBUG_MMU
        std::cerr << "[DEBUG] [MMU] pte_3.ppn = " << pte_3.GetPPN() << std::endl;
        std::cerr << "[DEBUG] [MMU] vaddr.vpn2 = " << vaddr.GetVPN2() << std::endl;
        std::cerr << "[DEBUG] [MMU] pte_2_entry_addr = " << pte_3.GetPPN() * VPAGE_SIZE + vaddr.GetVPN2() << std::endl;
#endif

        if (pte_2.GetX() == 0 && pte_2.GetW() == 0 && pte_2.GetR() == 0) // pointer to next level
        {
            exception = ValidatePTE(pte_2, 0);
            if (exception != MMU::Exception::NONE)
                return std::make_pair(paddr, exception);

            memory.Load(&pte_1, sizeof(pte_1), pte_2.GetPPN() * VPAGE_SIZE + vaddr.GetVPN1());

#ifdef DEBUG_MMU
            std::cerr << "[DEBUG] [MMU] pte_2.ppn = " << pte_2.GetPPN() << std::endl;
            std::cerr << "[DEBUG] [MMU] vaddr.vpn1 = " << vaddr.GetVPN1() << std::endl;
            std::cerr << "[DEBUG] [MMU] pte_1_entry_addr = " << pte_2.GetPPN() * VPAGE_SIZE + vaddr.GetVPN1()
                      << std::endl;
#endif

            if (pte_1.GetX() == 0 && pte_1.GetW() == 0 && pte_1.GetR() == 0) // pointer to next level
            {
                exception = ValidatePTE(pte_1, 0);
                if (exception != MMU::Exception::NONE)
                    return std::make_pair(paddr, exception);

                memory.Load(&pte_0, sizeof(pte_0), pte_1.GetPPN() * VPAGE_SIZE + vaddr.GetVPN0());

#ifdef DEBUG_MMU
                std::cerr << "[DEBUG] [MMU] pte_1.ppn = " << pte_1.GetPPN() << std::endl;
                std::cerr << "[DEBUG] [MMU] vaddr.vpn0 = " << vaddr.GetVPN0() << std::endl;
                std::cerr << "[DEBUG] [MMU] pte_0_entry_addr = " << pte_1.GetPPN() * VPAGE_SIZE + vaddr.GetVPN0()
                          << std::endl;
#endif

                exception = ValidatePTE(pte_0, rwx_flags);

#ifdef DEBUG_MMU
                std::cerr << "[DEBUG] [MMU] pte_0_entry->ppn exception: " << (int)exception << std::endl;
#endif

                if (exception != MMU::Exception::NONE)
                    return std::make_pair(paddr, exception);

                paddr.SetPageOffset(vaddr.GetPageOffset());
                paddr.SetPPN(pte_0.GetPPN());

#ifdef DEBUG_MMU
                std::bitset<bitops::BitSizeof<paddr_t>()> paddr_bitset(paddr.value);
                std::cerr << "[DEBUG] [MMU] paddr = " << paddr_bitset << std::endl;
                std::cerr << "[DEBUG] [MMU] paddr = 0x" << std::hex << paddr.value << std::dec << std::endl;
                std::cerr << "[DEBUG] [MMU: End]" << std::endl;
#endif

            } else // TODO: support other types of pages
                return std::make_pair(paddr, MMU::Exception::INVALID_PAGE_SIZE);
        } else // TODO: support other types of pages
            return std::make_pair(paddr, MMU::Exception::INVALID_PAGE_SIZE);
    } else // TODO: support other types of pages
        return std::make_pair(paddr, MMU::Exception::INVALID_PAGE_SIZE);

    return std::make_pair(paddr, MMU::Exception::NONE);
}

} // namespace rvsim
