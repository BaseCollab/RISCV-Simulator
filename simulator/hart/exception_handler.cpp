#include "exception_handler.h"

#include <cassert>
#include <elf.h>

namespace rvsim {

void ExceptionHandler::MMUExceptionHandler(Hart *hart, MMU::Exception exception)
{
    if (exception == MMU::Exception::NONE) {
        return;
    }

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

            break;

        case MMU::Exception::PAGE_WRITE_NO_READ:

            break;

        case MMU::Exception::PAGE_ACCESS_READ:

            break;

        case MMU::Exception::PAGE_ACCESS_WRITE:

            break;

        case MMU::Exception::PAGE_ACCESS_EXECUTE:

            break;

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
