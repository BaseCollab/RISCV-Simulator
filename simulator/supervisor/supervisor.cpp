#include "supervisor.h"

#include <cstring>
#include <cassert>

namespace rvsim {

/* static */
Supervisor::ExceptionHandlers Supervisor::handlers_;

Supervisor::Supervisor(Hart *hart, MemoryCtl *memory) : hart_(hart), memory_(memory)
{
    assert(hart);
    assert(memory);

    CSRs *csr_regs = &(hart_->csr_regs);

    InitializeCSR(csr_regs);
    LocateRootPageTable(csr_regs);
}

void Supervisor::InitializeCSR(CSRs *csr_regs)
{
    assert(csr_regs);

    csr_satp_t satp = {.mode = 0x9, .asid = 0x0, .ppn = root_page_number_}; // 0x9 = Sv48
    csr_t satp_reg = 0;
    std::memcpy(&satp_reg, &satp, sizeof(csr_satp_t));

    csr_regs->StoreCSR(CSR_SATP_IDX, satp_reg);

    // TODO: map all CSRs to physical memory as specification requires
}

int Supervisor::LocateRootPageTable(CSRs *csr_regs)
{
    // TODO: locate root virtual page table to root_page_number_, blocked
    // TODO: by MemoryCtl interface (no write of the whole page)

    (void)csr_regs;

    return 0;
}

void Supervisor::LoadElfFile(const std::string &elf_pathname)
{
    ElfLoader elf_loader(elf_pathname);
    elf_loader.LoadElf(memory_, *hart_);
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

} // namespace rvsim
