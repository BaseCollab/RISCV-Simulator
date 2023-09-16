#include "supervisor.h"

#include <cstring>

namespace rvsim {

Supervisor::Supervisor(CSRs *csr_regs, MemoryCtl *memory)
{
    InitializeCSR(csr_regs);
    LocateRootPageTable(csr_regs, memory);
}

void Supervisor::InitializeCSR(CSRs *csr_regs)
{
    csr_satp satp = {.mode = 0x9, .asid = 0x0, .ppn = root_page_number_}; // 0x9 = Sv48
    csr_t satp_reg = 0;
    std::memcpy(&satp_reg, &satp, sizeof(csr_satp));

    csr_regs->StoreCSR(CSR_SATP_IDX, satp_reg);

    // TODO: map all CSRs to physical memory as specification requires
}

int Supervisor::LocateRootPageTable(CSRs *csr_regs, MemoryCtl *memory)
{
    // TODO: locate root virtual page table to root_page_number_, blocked
    // TODO: by MemoryCtl interface (no write of the whole page)

    (void)csr_regs;
    (void)memory;

    return 0;
}

} // namespace rvsim
