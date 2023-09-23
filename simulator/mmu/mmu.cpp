#include "mmu.h"

#include <optional>
#include <utility>
#include <cstring>

namespace rvsim {

MMU::Exception MMU::ValidatePTE(const pte_t &pte, MMU::Target target) const
{
    if (pte.v == 0) // not valid page table entry
        return MMU::Exception::INVALID_PAGE_ENTRY;

    if (pte.w == 1 && pte.r == 0)
        return MMU::Exception::PAGE_WRITE_NO_READ;

    if (target != MMU::Target::NONE) {
        if ((target == MMU::Target::READ) && pte.r == 0)
            return MMU::Exception::PAGE_ACCESS_READ;

        if ((target == MMU::Target::WRITE) && pte.w == 0)
            return MMU::Exception::PAGE_ACCESS_WRITE;

        if ((target == MMU::Target::EXECUTE) && pte.x == 0)
            return MMU::Exception::PAGE_ACCESS_EXECUTE;
    }

    // TODO: add all other possible exceptions

    return MMU::Exception::NONE;
}

std::pair<paddr_t, std::optional<MMU::Exception>> MMU::VirtToPhysAddr(vaddr_t vaddr, MMU::Target target,
                                                                      const CSRs &csr_regs,
                                                                      const MemoryCtl &memory) const
{
    paddr_t paddr = {.value = 0};

    pte_t pte_3;
    pte_t pte_2;
    pte_t pte_1;
    pte_t pte_0;

    MMU::Exception exception;

    csr_t satp_reg = csr_regs.LoadCSR(CSR_SATP_IDX);
    csr_satp_t satp;
    std::memcpy(&satp_reg, &satp, sizeof(satp_reg));

    memory.Load(&pte_3, sizeof(pte_3), satp.ppn * VPAGE_SIZE + vaddr.fields.vpn_3);

    if (pte_3.x == 0 && pte_3.w == 0 && pte_3.r == 0) // pointer to next level
    {
        exception = ValidatePTE(pte_3, MMU::Target::NONE);
        if (exception != MMU::Exception::NONE)
            return std::make_pair(paddr, exception);

        memory.Load(&pte_2, sizeof(pte_2), pte_3.ppn * VPAGE_SIZE + vaddr.fields.vpn_2);

        if (pte_2.x == 0 && pte_2.w == 0 && pte_2.r == 0) // pointer to next level
        {
            exception = ValidatePTE(pte_2, MMU::Target::NONE);
            if (exception != MMU::Exception::NONE)
                return std::make_pair(paddr, exception);

            memory.Load(&pte_1, sizeof(pte_1), pte_2.ppn * VPAGE_SIZE + vaddr.fields.vpn_1);

            if (pte_1.x == 0 && pte_1.w == 0 && pte_1.r == 0) // pointer to next level
            {
                exception = ValidatePTE(pte_1, MMU::Target::NONE);
                if (exception != MMU::Exception::NONE)
                    return std::make_pair(paddr, exception);

                memory.Load(&pte_0, sizeof(pte_0), pte_1.ppn * VPAGE_SIZE + vaddr.fields.vpn_0);

                exception = ValidatePTE(pte_0, target);
                if (exception != MMU::Exception::NONE)
                    return std::make_pair(paddr, exception);

                paddr.fields.page_offset = vaddr.fields.page_offset;
                paddr.fields.ppn = pte_0.ppn;
            } else // TODO: support other types of pages
                return std::make_pair(paddr, MMU::Exception::INVALID_PAGE_SIZE);
        } else // TODO: support other types of pages
            return std::make_pair(paddr, MMU::Exception::INVALID_PAGE_SIZE);
    } else // TODO: support other types of pages
        return std::make_pair(paddr, MMU::Exception::INVALID_PAGE_SIZE);

    return std::make_pair(paddr, MMU::Exception::NONE);
}

} // namespace rvsim
