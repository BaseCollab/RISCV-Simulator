#include "exception_handler.h"

#include <cassert>
#include <elf.h>
#include <sysexits.h>
#include <err.h>

namespace rvsim {

void ExceptionHandler::MMUExceptionHandler(Hart *hart, MMU::Exception exception, uint8_t rwx_flags)
{
    (void)hart;
    (void)rwx_flags;

    if (exception == MMU::Exception::NONE) {
        return;
    }

    addr_t exception_addr = hart->GetPC();

    switch (exception) {
        case MMU::Exception::INVALID_PAGE_ENTRY:
            /*
                1) pass virtual address as argument here
                2) from virtual address get virtual page number
                3) from satp get pointer to root page table
                4) read element from root page table by [vpn]
                5) initialize this element
                6) create new table, request from memory new page
                7) set ppn
            */
           break;

        case MMU::Exception::INVALID_PAGE_SIZE:
            err(EX_SOFTWARE, "Superpages, megapages, etc - are not supported (rwx != 0x0 for not final page table entry) [addr %lx]", exception_addr);

        case MMU::Exception::PAGE_WRITE_NO_READ:
            err(EX_SOFTWARE, "Page invalid rights: write permission without read permission [addr %lx]", exception_addr);

        case MMU::Exception::PAGE_ACCESS_READ:
            err(EX_SOFTWARE, "Page access fault: read without read permission [addr %lx]", exception_addr);

        case MMU::Exception::PAGE_ACCESS_WRITE:
            err(EX_SOFTWARE, "Page access fault: write without write permission [addr %lx]", exception_addr);

        case MMU::Exception::PAGE_ACCESS_EXECUTE:
            err(EX_SOFTWARE, "Page access fault: execute without execute permission [addr %lx]", exception_addr);

        default:
            break;
    }
}

void ExceptionHandler::VirtualPageMapping(vaddr_t vaddr, uint8_t rwx_flags)
{
    (void)vaddr;
    (void)rwx_flags;
    // pte_t pte_3;
    // pte_t pte_2;
    // pte_t pte_1;
    // pte_t pte_0;

    // csr_t satp_reg = hart_->csr_regs.LoadCSR(CSR_SATP_IDX);
    // csr_satp_t satp;
    // std::memcpy(&satp_reg, &satp, sizeof(satp_reg));

    // memory_->Load(&pte_3, sizeof(pte_3), satp.ppn * VPAGE_SIZE + vaddr.fields.vpn_3);
}

} // namespace rvsim
