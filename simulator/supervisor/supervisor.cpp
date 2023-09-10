#include "supervisor.h"

#include <cstring>

namespace rvsim {

Supervisor::Supervisor(Hart &hart)
{
    csr_satp satp = {.mode = 0x9, .asid = 0x0, .ppn = root_page_number_}; // 0x9 = Sv48
    csr_t satp_reg = 0;
    std::memcpy(&satp_reg, &satp, sizeof(csr_satp));

    hart.storeCSR(CSR_SATP_IDX, satp_reg);
}

} // rvsim
