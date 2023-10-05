#include "supervisor.h"

#include <cstring>
#include <cassert>

namespace rvsim {

Supervisor::Supervisor(Hart *hart, PhysMemoryCtl *memory) : hart_(hart), memory_(memory)
{
    assert(hart);
    assert(memory);

    SetExceptionHandlers();
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
    Hart::ExceptionHandlers handlers;
    handlers.mmu_handler = std::bind(&ExceptionHandler::MMUExceptionHandler, hart_, std::placeholders::_1);

    hart.SetExceptionHandlers(handlers);
}

} // namespace rvsim
