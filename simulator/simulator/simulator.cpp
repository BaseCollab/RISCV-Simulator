#include "simulator/simulator.h"
#include "simulator/elf_loader.h"

#include <cstring>
#include <cassert>
#include <iostream>

namespace rvsim {

Simulator::Simulator(Hart *hart, PhysMemoryCtl *memory) : hart_(hart), memory_(memory)
{
    assert(hart);
    assert(memory);

    SetExceptionHandlers();
}

void Simulator::InitializeCSR(CSRs *csr_regs, reg_t root_page_idx)
{
    assert(csr_regs);

    csr_satp_t satp = {.mode = 0x9, .asid = 0x0, .ppn = root_page_idx}; // 0x9 = Sv48
    csr_t satp_reg;
    std::memcpy(&satp_reg, &satp, sizeof(csr_satp_t));

    csr_regs->StoreCSR(CSR_SATP_IDX, satp_reg);

    // TODO: map all CSRs to physical memory as specification requires
}

reg_t Simulator::AllocRootPageTable()
{
    auto page_idx_pair = memory_->GetCleanPage();
    reg_t page_idx = page_idx_pair.first;

    vpt_t vpt;
    memory_->StoreByPageIdx(page_idx, &vpt, sizeof(vpt));

#ifdef DEBUG
    std::cerr << "[DEBUG] Root page table is allocated at: phys page index = " << page_idx
              << ", phys address = " << page_idx * VPAGE_SIZE << std::endl;
#endif

    return page_idx;
}

void Simulator::LoadElfFile(const std::string &elf_pathname)
{
    hart_->SetGPR(0x2, STACK_PTR);
    PreparePageTable();

    ElfLoader elf_loader(elf_pathname);
    elf_loader.LoadElf(*this, *hart_);

    hart_->SetPC(elf_loader.GetElfEntryPoint());
}

void Simulator::SetExceptionHandlers()
{
    Hart::ExceptionHandlers handlers;
    handlers.mmu_handler = std::bind(&ExceptionHandler::MMUExceptionHandler, hart_, memory_, std::placeholders::_1,
                                     std::placeholders::_2);

    hart_->SetExceptionHandlers(handlers);
}

void Simulator::PreparePageTable()
{
    CSRs *csr_regs = &(hart_->csr_regs);

    reg_t root_page_idx = AllocRootPageTable();
    InitializeCSR(csr_regs, root_page_idx);

    csr_t satp_reg = hart_->csr_regs.LoadCSR(CSR_SATP_IDX);
    csr_satp_t satp;
    std::memcpy(&satp, &satp_reg, sizeof(satp_reg));

    //

    // CreatePageTableWalk(satp, vaddr);
}

template <bool IsLastLevel>
dword_t Simulator::CreatePageTableLVL(dword_t ppn_lvl, dword_t vpn, uint8_t rwx_flags) const
{
    pte_t pte;
    dword_t ppn_new {0};

    memory_->Load(&pte, sizeof(pte), ppn_lvl * VPAGE_SIZE + vpn);

    if (pte.GetV() == 0) {
        auto page_idx_pair = memory_->GetCleanPage();
        reg_t page_idx = page_idx_pair.first;

        vpt_t vpt;
        memory_->StoreByPageIdx(page_idx, &vpt, sizeof(vpt));

        pte.SetPPN(page_idx);
        pte.SetV(1);
        ppn_new = page_idx;
        
        if constexpr (IsLastLevel == true) {
            pte.SetR(!!(rwx_flags & PF_R));
            pte.SetW(!!(rwx_flags & PF_W));
            pte.SetX(!!(rwx_flags & PF_X));
        }

        memory_->Store(ppn_new * VPAGE_SIZE + vpn, &pte, sizeof(pte));
    }

    return ppn_new;
}

// clang-format off
void Simulator::CreatePageTableWalk(vaddr_t vaddr) const
{
    csr_t satp_reg = hart_->csr_regs.LoadCSR(CSR_SATP_IDX);
    csr_satp_t satp;
    std::memcpy(&satp, &satp_reg, sizeof(satp_reg));

    dword_t ppn_3 = CreatePageTableLVL<false>(satp.ppn, vaddr.GetVPN3());
    dword_t ppn_2 = CreatePageTableLVL<false>(ppn_3,    vaddr.GetVPN2());
    dword_t ppn_1 = CreatePageTableLVL<false>(ppn_2,    vaddr.GetVPN1());
                    CreatePageTableLVL<true> (ppn_1,    vaddr.GetVPN0());
}
// clang-format on

} // namespace rvsim
