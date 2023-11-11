#include "simulator/simulator.h"
#include "simulator/elf_loader.h"

#include <cstring>
#include <cassert>
#include <iostream>
#include <bitset>

namespace rvsim {

Simulator::Simulator(Hart *hart, PhysMemoryCtl *memory, size_t n_stack_pages)
    : n_stack_pages_(n_stack_pages), hart_(hart), memory_(memory)
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
    handlers.mmu_handler =
        std::bind(&ExceptionHandler::MMUExceptionHandler, hart_, memory_, std::placeholders::_1, std::placeholders::_2);

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
}

template <bool IsLastLevel>
dword_t Simulator::CreatePageTableLVL(dword_t ppn_lvl, dword_t vpn, uint8_t rwx_flags) const
{
    pte_t pte;
    dword_t ppn_new {0};

    memory_->Load(&pte, sizeof(pte), ppn_lvl * VPAGE_SIZE + vpn);

    if (pte.GetV() == 0) {
#ifdef DEBUG_EXCEPTION
        std::cerr << "[DEBUG] [MMU] Invalid PTE at 0x" << std::hex << ppn_lvl * VPAGE_SIZE + vpn << std::endl;
#endif

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

        memory_->Store(ppn_lvl * VPAGE_SIZE + vpn, &pte, sizeof(pte));
    }

    return ppn_new;
}

// clang-format off
void Simulator::MapVirtualPage(vaddr_t page_vaddr, uint8_t rwx_flags) const
{
#ifdef DEBUG_MMU
    std::cerr << "[DEBUG] [PT alloc: Start mapping] " << std::endl;

    std::bitset<bitops::BitSizeof<vaddr_t>()> vaddr_bitset(page_vaddr);
    std::cerr << "[DEBUG] [PT alloc] vaddr = " << vaddr_bitset << std::endl;
    std::cerr << "[DEBUG] [PT alloc] vaddr = 0x" << std::hex << page_vaddr.value << std::dec << std::endl;

    std::cerr << "[DEBUG] [PT alloc] vaddr.offset = " << page_vaddr.GetPageOffset() << std::endl;
#endif

    csr_t satp_reg = hart_->csr_regs.LoadCSR(CSR_SATP_IDX);
    csr_satp_t satp;
    std::memcpy(&satp, &satp_reg, sizeof(satp_reg));
#ifdef DEBUG_EXCEPTION
    std::cerr << "[DEBUG] [PT alloc] satp.ppn = 0x" << satp.ppn << std::endl;
#endif

    dword_t ppn_3 = CreatePageTableLVL<false>(satp.ppn, page_vaddr.GetVPN3(), rwx_flags);
#ifdef DEBUG_EXCEPTION
    std::cerr << "[DEBUG] [PT alloc] ppn_3 = 0x" << ppn_3 << std::endl;
#endif

    dword_t ppn_2 = CreatePageTableLVL<false>(ppn_3,    page_vaddr.GetVPN2(), rwx_flags);
#ifdef DEBUG_EXCEPTION
    std::cerr << "[DEBUG] [PT alloc] ppn_2 = 0x" << ppn_2 << std::endl;
#endif

    dword_t ppn_1 = CreatePageTableLVL<false>(ppn_2,    page_vaddr.GetVPN1(), rwx_flags);
#ifdef DEBUG_EXCEPTION
    std::cerr << "[DEBUG] [PT alloc] ppn_1 = 0x" << ppn_1 << std::endl;
#endif

    dword_t ppn_0 = CreatePageTableLVL<true> (ppn_1,    page_vaddr.GetVPN0(), rwx_flags);
#ifdef DEBUG_EXCEPTION
    std::cerr << "[DEBUG] [PT alloc] ppn_0 = 0x" << ppn_0 << std::endl;
#else
    (void)ppn_0;
#endif

#ifdef DEBUG_EXCEPTION
    std::cerr << "[DEBUG] [PT alloc: End mapping]" << std::dec << std::endl;
#endif
}
// clang-format on

// TODO: rewrite the whole function
void Simulator::MapVirtualRange(vaddr_t vaddr_start, vaddr_t vaddr_end, uint8_t rwx_flags) const
{
    addr_t vaddr = vaddr_start;
    for (; vaddr < vaddr_end; vaddr += VPAGE_SIZE) {
        MapVirtualPage(vaddr, rwx_flags);
    }

    addr_t vpage_padding = (VPAGE_SIZE - vaddr % VPAGE_SIZE) % VPAGE_SIZE;
    vpage_padding = (vpage_padding == 0) ? VPAGE_SIZE : vpage_padding;

    if ((vaddr_end - vaddr + VPAGE_SIZE) > vpage_padding) {
        MapVirtualPage(vaddr_end, rwx_flags);
    }
}

} // namespace rvsim
