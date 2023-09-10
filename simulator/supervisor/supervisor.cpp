#include "supervisor.h"

#include <cstring>

namespace rvsim {

Supervisor::Supervisor(Hart *hart, MemoryCtl *memory)
{
    initializeCSR(hart);
    locateRootPageTable(hart, memory);
}

void Supervisor::initializeCSR(Hart *hart)
{
    csr_satp satp = {.mode = 0x9, .asid = 0x0, .ppn = root_page_number_}; // 0x9 = Sv48
    csr_t satp_reg = 0;
    std::memcpy(&satp_reg, &satp, sizeof(csr_satp));

    hart->StoreCSR(CSR_SATP_IDX, satp_reg);

    // TODO: mapp all CSRs to physical memory as specification requires
}

int Supervisor::locateRootPageTable(Hart *hart, MemoryCtl *memory)
{
    // TODO: locate root virtual page table to root_page_number_, blocked
    // TODO: by MemoryCtl interface (no write of the whole page)

    return 0;
}

} // rvsim
