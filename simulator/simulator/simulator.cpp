#include "simulator.h"

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
    PreparePageTable();

    ElfLoader elf_loader(elf_pathname);
    elf_loader.LoadElf(*hart_);

    hart_->SetPC(elf_loader.GetElfEntryPoint());
    hart_->SetGPR(0x2, STACK_PTR);
}

void Simulator::SetExceptionHandlers()
{
    Hart::ExceptionHandlers handlers;
    handlers.mmu_handler = std::bind(&ExceptionHandler::MMUExceptionHandler, hart_, memory_, std::placeholders::_1,
                                     std::placeholders::_2, std::placeholders::_3);

    hart_->SetExceptionHandlers(handlers);
}

void Simulator::PreparePageTable()
{
    CSRs *csr_regs = &(hart_->csr_regs);

    reg_t root_page_idx = AllocRootPageTable();
    InitializeCSR(csr_regs, root_page_idx);
}

dword_t Simulator::CreatePageTableLVL(dword_t ppn_lvl, dword_t vpn)
{
    pte_t pte;
    dword_t ppn_new {0};

    memory->Load(&pte, sizeof(pte), ppn_lvl * VPAGE_SIZE + vpn);

    if (pte.GetV() == 0) {
        auto page_idx_pair = memory->GetCleanPage();
        reg_t page_idx = page_idx_pair.first;

        vpt_t vpt;
        memory->StoreByPageIdx(page_idx, &vpt, sizeof(vpt));

        ppn_new = page_idx;
        pte.SetPPN(page_idx);
        pte.SetV(1);

        memory->Store(ppn_new * VPAGE_SIZE + vpn, &pte, sizeof(pte));
    }

    return ppn_new;
}

void Simulator::VirtualPageMapping(vaddr_t page_vaddr)
{
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

    if (pte_3.GetV() == 0) {
        auto page_idx_pair = memory->GetCleanPage();
        reg_t page_idx = page_idx_pair.first;

        vpt_t vpt;
        memory->StoreByPageIdx(page_idx, &vpt, sizeof(vpt));

        ppn_3 = page_idx;
        pte_3.SetPPN(page_idx);
        pte_3.SetV(1);

        memory->Store(satp.ppn * VPAGE_SIZE + vaddr.GetVPN3(), &pte_3, sizeof(pte_3));

        ppn_3 = pte_3.GetPPN();
    }

    memory->Load(&pte_2, sizeof(pte_2), ppn_3 * VPAGE_SIZE + vaddr.GetVPN2());

    if (pte_2.GetV() == 0) {
        auto page_idx_pair = memory->GetCleanPage();
        reg_t page_idx = page_idx_pair.first;

        vpt_t vpt;
        memory->StoreByPageIdx(page_idx, &vpt, sizeof(vpt));

        ppn_2 = page_idx;
        pte_2.SetPPN(page_idx);
        pte_2.SetV(1);

        memory->Store(ppn_3 * VPAGE_SIZE + vaddr.GetVPN2(), &pte_2, sizeof(pte_2));
    } else {
        ppn_2 = pte_2.GetPPN();
    }

    memory->Load(&pte_1, sizeof(pte_1), ppn_2 * VPAGE_SIZE + vaddr.GetVPN1());

    if (pte_1.GetV() == 0) {
        auto page_idx_pair = memory->GetCleanPage();
        reg_t page_idx = page_idx_pair.first;

        vpt_t vpt;
        memory->StoreByPageIdx(page_idx, &vpt, sizeof(vpt));

        ppn_1 = page_idx;
        pte_1.SetPPN(page_idx);
        pte_1.SetV(1);

        memory->Store(ppn_2 * VPAGE_SIZE + vaddr.GetVPN1(), &pte_1, sizeof(pte_1));
    } else {
        ppn_1 = pte_1.GetPPN();
    }

    memory->Load(&pte_0, sizeof(pte_0), ppn_1 * VPAGE_SIZE + vaddr.GetVPN0());

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
    }
}

} // namespace rvsim
