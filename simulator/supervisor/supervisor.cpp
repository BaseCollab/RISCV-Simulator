#include "supervisor.h"

#include <cstring>
#include <cassert>

namespace rvsim {

/* static */
Supervisor::ExceptionHandlers Supervisor::handlers_;

Supervisor::Supervisor(Hart *hart, PhysMemoryCtl *memory) : hart_(hart), memory_(memory)
{
    assert(hart);
    assert(memory);
}

void Supervisor::InitializeCSR(CSRs *csr_regs, reg_t root_page_idx)
{
    assert(csr_regs);

    csr_satp_t satp = {.mode = 0x9, .asid = 0x0, .ppn = root_page_idx}; // 0x9 = Sv48
    csr_t satp_reg = 0;
    std::memcpy(&satp_reg, &satp, sizeof(csr_satp_t));

    csr_regs->StoreCSR(CSR_SATP_IDX, satp_reg);

    // TODO: map all CSRs to physical memory as specification requires
}

reg_t Supervisor::AllocRootPageTable()
{
    auto page_idx_pair = memory_->GetCleanPage();
    reg_t page_idx = page_idx_pair.first;

    vpt_t vpt;
    memory_->StoreByPageIdx(page_idx, &vpt, sizeof(vpt));

    return page_idx;
}

void Supervisor::LoadElfFile(const std::string &elf_pathname)
{
    CSRs *csr_regs = &(hart_->csr_regs);

    reg_t root_page_idx = AllocRootPageTable();
    InitializeCSR(csr_regs, root_page_idx);

    ElfLoader elf_loader(elf_pathname);
    elf_loader.LoadElf(*hart_);
}

void Supervisor::SetExceptionHandlers()
{
    handlers_.mmu_handler = std::bind(&Supervisor::MMUExceptionHandler, this, std::placeholders::_1);
}

void Supervisor::MMUExceptionHandler(MMU::Exception exception)
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

void Supervisor::VirtualPageMapping(vaddr_t vaddr, uint8_t rwx_flags)
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
