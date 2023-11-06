#include "exception_handler.h"

#include <iostream>
#include <bitset>
#include <cassert>
#include <elf.h>
#include <sysexits.h>
#include <err.h>

namespace rvsim {

void ExceptionHandler::MMUExceptionHandler(Hart *hart, PhysMemoryCtl *memory, MMU::Exception exception, addr_t vaddr,
                                           uint8_t rwx_flags)
{
    if (exception == MMU::Exception::NONE) {
        return;
    }

    switch (exception) {
        case MMU::Exception::INVALID_PAGE_ENTRY:
            VirtualPageMapping(hart, memory, vaddr, rwx_flags);
            break;

        case MMU::Exception::INVALID_PAGE_SIZE:
            err(EX_SOFTWARE,
                "Superpages, megapages, etc - are not supported (rwx != 0x0 for not final page table entry) [addr %lx]",
                vaddr);

        case MMU::Exception::PAGE_WRITE_NO_READ:
            err(EX_SOFTWARE, "Page invalid rights: write permission without read permission [addr %lx]", vaddr);

        case MMU::Exception::PAGE_ACCESS_READ:
            err(EX_SOFTWARE, "Page access fault: read without read permission [addr %lx]", vaddr);

        case MMU::Exception::PAGE_ACCESS_WRITE:
            err(EX_SOFTWARE, "Page access fault: write without write permission [addr %lx]", vaddr);

        case MMU::Exception::PAGE_ACCESS_EXECUTE:
            err(EX_SOFTWARE, "Page access fault: execute without execute permission [addr %lx]", vaddr);

        default:
            break;
    }
}

void ExceptionHandler::VirtualPageMapping(Hart *hart, PhysMemoryCtl *memory, vaddr_t vaddr, uint8_t rwx_flags)
{
#ifdef DEBUG_EXCEPTION
    std::cerr << "[DEBUG] [EXCEPTION: INVALID_PAGE_ENTRY] Start restoring VPT" << std::endl;

    std::bitset<bitops::BitSizeof<vaddr_t>()> vaddr_bitset(vaddr.value);
    std::cerr << "[DEBUG] [EXCEPTION] vaddr = " << vaddr_bitset << std::endl;
    std::cerr << "[DEBUG] [EXCEPTION] vaddr = 0x" << std::hex << vaddr.value << std::dec << std::endl;

    std::cerr << "[DEBUG] [EXCEPTION] vaddr.offset = " << vaddr.GetPageOffset() << std::endl;
#endif

    pte_t pte_3;
    pte_t pte_2;
    pte_t pte_1;
    pte_t pte_0;

    reg_t ppn_3;
    reg_t ppn_2;
    reg_t ppn_1;

    csr_t satp_reg = hart->csr_regs.LoadCSR(CSR_SATP_IDX);
    csr_satp_t satp;
    std::memcpy(&satp, &satp_reg, sizeof(satp_reg));

    memory->Load(&pte_3, sizeof(pte_3), satp.ppn * VPAGE_SIZE + vaddr.GetVPN3());

#ifdef DEBUG_EXCEPTION
    std::cerr << "[DEBUG] [EXCEPTION] satp.ppn = " << satp.ppn << ", vaddr.vpn3 = " << vaddr.GetVPN3()
              << " => pte_3_entry_addr = " << satp.ppn * VPAGE_SIZE + vaddr.GetVPN3() << std::endl;
#endif

    if (pte_3.GetV() == 0) {
        auto page_idx_pair = memory->GetCleanPage();
        reg_t page_idx = page_idx_pair.first;

        vpt_t vpt;
        memory->StoreByPageIdx(page_idx, &vpt, sizeof(vpt));

        ppn_3 = page_idx;
        pte_3.SetPPN(page_idx);
        pte_3.SetV(1);

        memory->Store(satp.ppn * VPAGE_SIZE + vaddr.GetVPN3(), &pte_3, sizeof(pte_3));

#ifdef DEBUG_EXCEPTION
        std::cerr << "[DEBUG] [EXCEPTION] pte_3_entry is invalid" << std::endl;
        std::cerr << "[DEBUG] [EXCEPTION] Store PPN " << page_idx
                  << " to pte_3_entry = " << satp.ppn * VPAGE_SIZE + vaddr.GetVPN3() << std::endl;
#endif
    } else {
#ifdef DEBUG_EXCEPTION
        std::cerr << "[DEBUG] [EXCEPTION] pte_3_entry is valid" << std::endl;
#endif

        ppn_3 = pte_3.GetPPN();
    }

    memory->Load(&pte_2, sizeof(pte_2), ppn_3 * VPAGE_SIZE + vaddr.GetVPN2());

#ifdef DEBUG_EXCEPTION
    std::cerr << "[DEBUG] [EXCEPTION] pte_3_entry.ppn = " << ppn_3 << ", vaddr.vpn2 = " << vaddr.GetVPN2()
              << " => pte_2_entry_addr = " << ppn_3 * VPAGE_SIZE + vaddr.GetVPN2() << std::endl;
#endif

    if (pte_2.GetV() == 0) {
        auto page_idx_pair = memory->GetCleanPage();
        reg_t page_idx = page_idx_pair.first;

        vpt_t vpt;
        memory->StoreByPageIdx(page_idx, &vpt, sizeof(vpt));

        ppn_2 = page_idx;
        pte_2.SetPPN(page_idx);
        pte_2.SetV(1);

        memory->Store(ppn_3 * VPAGE_SIZE + vaddr.GetVPN2(), &pte_2, sizeof(pte_2));

#ifdef DEBUG_EXCEPTION
        std::cerr << "[DEBUG] [EXCEPTION] pte_2_entry is invalid" << std::endl;
        std::cerr << "[DEBUG] [EXCEPTION] Store PPN " << page_idx
                  << " to pte_2_entry = " << ppn_3 * VPAGE_SIZE + vaddr.GetVPN2() << std::endl;
#endif
    } else {
#ifdef DEBUG_EXCEPTION
        std::cerr << "[DEBUG] [EXCEPTION] pte_2_entry is valid" << std::endl;
#endif

        ppn_2 = pte_2.GetPPN();
    }

    memory->Load(&pte_1, sizeof(pte_1), ppn_2 * VPAGE_SIZE + vaddr.GetVPN1());

#ifdef DEBUG_EXCEPTION
    std::cerr << "[DEBUG] [EXCEPTION] pte_2_entry.ppn = " << ppn_2 << ", vaddr.vpn1 = " << vaddr.GetVPN1()
              << " => pte_1_entry_addr = " << ppn_2 * VPAGE_SIZE + vaddr.GetVPN1() << std::endl;
#endif

    if (pte_1.GetV() == 0) {
        auto page_idx_pair = memory->GetCleanPage();
        reg_t page_idx = page_idx_pair.first;

        vpt_t vpt;
        memory->StoreByPageIdx(page_idx, &vpt, sizeof(vpt));

        ppn_1 = page_idx;
        pte_1.SetPPN(page_idx);
        pte_1.SetV(1);

        memory->Store(ppn_2 * VPAGE_SIZE + vaddr.GetVPN1(), &pte_1, sizeof(pte_1));

#ifdef DEBUG_EXCEPTION
        std::cerr << "[DEBUG] [EXCEPTION] pte_1_entry is invalid" << std::endl;
        std::cerr << "[DEBUG] [EXCEPTION] Store PPN " << page_idx
                  << " to pte_1_entry = " << ppn_2 * VPAGE_SIZE + vaddr.GetVPN1() << std::endl;
#endif
    } else {
#ifdef DEBUG_EXCEPTION
        std::cerr << "[DEBUG] [EXCEPTION] pte_1_entry is valid" << std::endl;
#endif

        ppn_1 = pte_1.GetPPN();
    }

    memory->Load(&pte_0, sizeof(pte_0), ppn_1 * VPAGE_SIZE + vaddr.GetVPN0());

#ifdef DEBUG_EXCEPTION
    std::cerr << "[DEBUG] [EXCEPTION] pte_1_entry.ppn = " << ppn_1 << ", vaddr.vpn0 = " << vaddr.GetVPN0()
              << " => pte_0_entry_addr = " << ppn_1 * VPAGE_SIZE + vaddr.GetVPN0() << std::endl;
#endif

    if (pte_0.GetV() == 0) {
        auto page_idx_pair = memory->GetCleanPage();
        reg_t page_idx = page_idx_pair.first;

        vpt_t vpt;
        memory->StoreByPageIdx(page_idx, &vpt, sizeof(vpt));

        pte_0.SetPPN(page_idx);
        pte_0.SetV(1);

        pte_0.SetR(!!(rwx_flags & PF_R));
        pte_0.SetW(!!(rwx_flags & PF_W));
        pte_0.SetX(!!(rwx_flags & PF_X));

        memory->Store(ppn_1 * VPAGE_SIZE + vaddr.GetVPN0(), &pte_0, sizeof(pte_0));

#ifdef DEBUG_EXCEPTION
        std::cerr << "[DEBUG] [EXCEPTION] pte_0_entry is invalid" << std::endl;
        std::cerr << "[DEBUG] [EXCEPTION] Store PPN " << page_idx
                  << " to pte_0_entry = " << ppn_1 * VPAGE_SIZE + vaddr.GetVPN0() << std::endl;
#endif
    }

#ifdef DEBUG_EXCEPTION
    std::cerr << "[DEBUG] [EXCEPTION: INVALID_PAGE_ENTRY] End restoring VPT" << std::endl;
#endif
}

} // namespace rvsim
