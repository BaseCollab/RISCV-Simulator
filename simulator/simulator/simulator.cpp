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

// clang-format off
void Simulator::InitializeCSR(CSRs *csr_regs, reg_t root_page_idx)
{
    assert(csr_regs);

    CSRs::satp_t satp;
    satp.fields.mode = 0x9; // 0x9 = Sv48
    satp.fields.asid = 0x0;
    satp.fields.ppn = root_page_idx;

    CSRs::mstatus_t mstatus;
    mstatus.fields.mxr = 0x1; // Make eXecutable Readable
    mstatus.fields.sum = 0x1;
    mstatus.fields.mprv = 0x0;

    csr_regs->StoreCSR<CSRs::satp_t>   (CSRs::Index::SATP,    satp);
    csr_regs->StoreCSR<CSRs::mstatus_t>(CSRs::Index::MSTATUS, mstatus);

    // TODO: map all CSRs to physical memory as specification requires
}
// clang-format on

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
    elf_loader.LoadElf(*this, hart_);

    hart_->SetPC(elf_loader.GetElfEntryPoint());
}

void Simulator::SetExceptionHandlers()
{
    Hart::ExceptionHandlers handlers;
    handlers.default_handler =
        std::bind(&ExceptionHandler::CallExceptionHandler, hart_, memory_, std::placeholders::_1, std::placeholders::_2);

    hart_->SetExceptionHandlers(handlers);
}

void Simulator::PreparePageTable()
{
    CSRs *csr_regs = &(hart_->csr_regs);

    reg_t root_page_idx = AllocRootPageTable();
    InitializeCSR(csr_regs, root_page_idx);

    for (size_t i = 1; i <= n_stack_pages_; ++i) {
        MapVirtualPage(STACK_PTR - VPAGE_SIZE * i);
    }
}

template <bool IsLastLevel>
dword_t Simulator::CreatePageTableLVL(dword_t ppn_lvl, dword_t vpn, uint8_t urwx_flags) const
{
    pte_t pte;
    dword_t ppn_new {0};

    memory_->Load(&pte, sizeof(pte), ppn_lvl * VPAGE_SIZE + vpn * sizeof(pte_t));

    if (pte.GetV() == 0) {
#ifdef DEBUG_EXCEPTION
        std::cerr << "[DEBUG] [PT alloc] Invalid PTE at 0x" << std::hex << ppn_lvl * VPAGE_SIZE + vpn * sizeof(pte_t)
                  << std::endl;
#endif

        auto page_idx_pair = memory_->GetCleanPage();
        reg_t page_idx = page_idx_pair.first;

        vpt_t vpt;
        memory_->StoreByPageIdx(page_idx, &vpt, sizeof(vpt));

        pte.SetPPN(page_idx);
        pte.SetV(1);
        ppn_new = page_idx;

        if constexpr (IsLastLevel == true) {
            pte.SetR(!!(urwx_flags & PF_R));
            pte.SetW(!!(urwx_flags & PF_W));
            pte.SetX(!!(urwx_flags & PF_X));
            pte.SetU(!!(urwx_flags & PF_U));
        }

        memory_->Store(ppn_lvl * VPAGE_SIZE + vpn * sizeof(pte_t), &pte, sizeof(pte_t));
    } else {
        ppn_new = pte.GetPPN();
    }

    return ppn_new;
}

// clang-format off
void Simulator::MapVirtualPage(vaddr_t page_vaddr, uint8_t urwx_flags) const
{
    CSRs *csr_regs = &(hart_->csr_regs);

#ifdef DEBUG_MMU
    std::cerr << "[DEBUG] [PT alloc: Start mapping] " << std::endl;

    std::bitset<bitops::BitSizeof<vaddr_t>()> vaddr_bitset(page_vaddr);
    std::cerr << "[DEBUG] [PT alloc] vaddr = " << vaddr_bitset << std::endl;
    std::cerr << "[DEBUG] [PT alloc] vaddr = 0x" << std::hex << page_vaddr.value << std::dec << std::endl;

    std::cerr << "[DEBUG] [PT alloc] vaddr.offset = " << page_vaddr.GetPageOffset() << std::endl;
#endif

    auto satp = csr_regs->LoadCSR<CSRs::satp_t>(CSRs::Index::SATP);
#ifdef DEBUG_EXCEPTION
    std::bitset<bitops::BitSizeof<hword_t>()> vpn_3(page_vaddr.GetVPN3());
    std::cerr << "[DEBUG] [PT alloc] 1) satp.ppn = 0x" << satp.fields.ppn << ", vpn_3 = 0b" << vpn_3 << std::endl;
#endif

    dword_t ppn_3 = CreatePageTableLVL<false>(satp.fields.ppn, page_vaddr.GetVPN3());
#ifdef DEBUG_EXCEPTION
    std::bitset<bitops::BitSizeof<hword_t>()> vpn_2(page_vaddr.GetVPN2());
    std::cerr << "[DEBUG] [PT alloc] 2) ppn_3 = 0x" << ppn_3 << ", vpn_2 = 0b" << vpn_2 << std::endl;
#endif

    dword_t ppn_2 = CreatePageTableLVL<false>(ppn_3, page_vaddr.GetVPN2());
#ifdef DEBUG_EXCEPTION
    std::bitset<bitops::BitSizeof<hword_t>()> vpn_1(page_vaddr.GetVPN1());
    std::cerr << "[DEBUG] [PT alloc] 3) ppn_2 = 0x" << ppn_2 << ", vpn_1 = 0b" << vpn_1 << std::endl;
#endif

    dword_t ppn_1 = CreatePageTableLVL<false>(ppn_2, page_vaddr.GetVPN1());
#ifdef DEBUG_EXCEPTION
    std::bitset<bitops::BitSizeof<hword_t>()> vpn_0(page_vaddr.GetVPN0());
    std::cerr << "[DEBUG] [PT alloc] 4) ppn_1 = 0x" << ppn_1 << ", vpn_0 = 0b" << vpn_0 << std::endl;
#endif

    dword_t ppn_0 = CreatePageTableLVL<true> (ppn_1, page_vaddr.GetVPN0(), urwx_flags);
#ifdef DEBUG_EXCEPTION
    std::cerr << "[DEBUG] [PT alloc] 5) ppn_0 = 0x" << ppn_0 << std::endl;
#else
    (void)ppn_0;
#endif

#ifdef DEBUG_EXCEPTION
    std::cerr << "[DEBUG] [PT alloc: End mapping]" << std::dec << std::endl;
#endif
}
// clang-format on

// TODO: rewrite the whole function
void Simulator::MapVirtualRange(vaddr_t vaddr_start, vaddr_t vaddr_end, uint8_t urwx_flags) const
{
    addr_t vaddr = vaddr_start;
    for (; vaddr < vaddr_end; vaddr += VPAGE_SIZE) {
        MapVirtualPage(vaddr, urwx_flags);
    }

    addr_t vpage_padding = (VPAGE_SIZE - vaddr % VPAGE_SIZE) % VPAGE_SIZE;
    vpage_padding = (vpage_padding == 0) ? VPAGE_SIZE : vpage_padding;

    if ((vaddr_end - vaddr + VPAGE_SIZE) > vpage_padding) {
        MapVirtualPage(vaddr_end, urwx_flags);
    }
}

Hart *Simulator::GetActiveHart()
{
    return hart_;
}

} // namespace rvsim
