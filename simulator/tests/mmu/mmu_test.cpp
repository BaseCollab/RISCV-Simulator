#include <gtest/gtest.h>

#include "common/macros.h"
#include "common/constants.h"
#include "common/config.h"
#include "hart/hart.h"
#include "simulator/simulator.h"
#include "memory/memory_controller.h"
#include "simulator/elf_loader.h"

namespace rvsim {

TEST(MMUTest, ComplexTest)
{
    {
        auto memory = PhysMemoryCtl();
        auto hart = Hart(&memory);
        auto sim = Simulator(&hart, &memory);

        sim.hart_->SetGPR(0x2, Simulator::STACK_PTR);
        CSRs *csr_regs = &(sim.hart_->csr_regs);

        reg_t root_page_idx = sim.AllocRootPageTable();
        sim.InitializeCSR(csr_regs, root_page_idx);

        auto satp = csr_regs->LoadCSR<CSRs::satp_t>(CSRs::Index::SATP);

        vaddr_t page_vaddr = Simulator::STACK_PTR - VPAGE_SIZE;
        sim.CreatePageTableLVL<true>(satp.fields.ppn, page_vaddr.GetVPN3());

        Exception exception = Exception::NONE;
        paddr_t paddr {0};

        std::tie(paddr, exception) = sim.hart_->mmu_.VirtToPhysAddr(page_vaddr, PF_W, *csr_regs, memory);
        ASSERT_EQ(exception, Exception::MMU_INVALID_PAGE_SIZE);
    }

    {
        auto memory = PhysMemoryCtl();
        auto hart = Hart(&memory);
        auto sim = Simulator(&hart, &memory);

        sim.hart_->SetGPR(0x2, Simulator::STACK_PTR);
        CSRs *csr_regs = &(sim.hart_->csr_regs);

        reg_t root_page_idx = sim.AllocRootPageTable();
        sim.InitializeCSR(csr_regs, root_page_idx);

        auto satp = csr_regs->LoadCSR<CSRs::satp_t>(CSRs::Index::SATP);

        vaddr_t page_vaddr = Simulator::STACK_PTR - VPAGE_SIZE;
        sim.CreatePageTableLVL<false>(satp.fields.ppn, page_vaddr.GetVPN3());

        Exception exception = Exception::NONE;
        paddr_t paddr {0};

        std::tie(paddr, exception) = sim.hart_->mmu_.VirtToPhysAddr(page_vaddr, PF_W, *csr_regs, memory);
        ASSERT_EQ(exception, Exception::MMU_INVALID_PAGE_ENTRY);
    }

    {
        auto memory = PhysMemoryCtl();
        auto hart = Hart(&memory);
        auto sim = Simulator(&hart, &memory);

        sim.hart_->SetGPR(0x2, Simulator::STACK_PTR);
        CSRs *csr_regs = &(sim.hart_->csr_regs);

        reg_t root_page_idx = sim.AllocRootPageTable();
        sim.InitializeCSR(csr_regs, root_page_idx);

        auto satp = csr_regs->LoadCSR<CSRs::satp_t>(CSRs::Index::SATP);

        vaddr_t page_vaddr = Simulator::STACK_PTR - VPAGE_SIZE;
        dword_t ppn_3 = sim.CreatePageTableLVL<false>(satp.fields.ppn, page_vaddr.GetVPN3());
        dword_t ppn_2 = sim.CreatePageTableLVL<false>(ppn_3, page_vaddr.GetVPN2());
        dword_t ppn_1 = sim.CreatePageTableLVL<false>(ppn_2, page_vaddr.GetVPN1());
        dword_t ppn_0 = sim.CreatePageTableLVL<true> (ppn_1, page_vaddr.GetVPN0(), PF_U | PF_W);

        (void)ppn_0;

        Exception exception = Exception::NONE;
        paddr_t paddr {0};

        std::tie(paddr, exception) = sim.hart_->mmu_.VirtToPhysAddr(page_vaddr, PF_W, *csr_regs, memory);
        ASSERT_EQ(exception, Exception::MMU_PAGE_WRITE_NO_READ);
    }

    {
        auto memory = PhysMemoryCtl();
        auto hart = Hart(&memory);
        auto sim = Simulator(&hart, &memory);

        sim.hart_->SetGPR(0x2, Simulator::STACK_PTR);
        CSRs *csr_regs = &(sim.hart_->csr_regs);

        reg_t root_page_idx = sim.AllocRootPageTable();
        sim.InitializeCSR(csr_regs, root_page_idx);

        auto satp = csr_regs->LoadCSR<CSRs::satp_t>(CSRs::Index::SATP);

        vaddr_t page_vaddr = Simulator::STACK_PTR - VPAGE_SIZE;
        dword_t ppn_3 = sim.CreatePageTableLVL<false>(satp.fields.ppn, page_vaddr.GetVPN3());
        dword_t ppn_2 = sim.CreatePageTableLVL<false>(ppn_3, page_vaddr.GetVPN2());
        dword_t ppn_1 = sim.CreatePageTableLVL<false>(ppn_2, page_vaddr.GetVPN1());
        dword_t ppn_0 = sim.CreatePageTableLVL<true> (ppn_1, page_vaddr.GetVPN0(), PF_U | PF_R);

        (void)ppn_0;

        Exception exception = Exception::NONE;
        paddr_t paddr {0};

        std::tie(paddr, exception) = sim.hart_->mmu_.VirtToPhysAddr(page_vaddr, PF_W, *csr_regs, memory);
        ASSERT_EQ(exception, Exception::MMU_PAGE_ACCESS_WRITE);
    }

    {
        auto memory = PhysMemoryCtl();
        auto hart = Hart(&memory);
        auto sim = Simulator(&hart, &memory);

        sim.hart_->SetGPR(0x2, Simulator::STACK_PTR);
        CSRs *csr_regs = &(sim.hart_->csr_regs);

        reg_t root_page_idx = sim.AllocRootPageTable();
        sim.InitializeCSR(csr_regs, root_page_idx);

        auto satp = csr_regs->LoadCSR<CSRs::satp_t>(CSRs::Index::SATP);

        vaddr_t page_vaddr = Simulator::STACK_PTR - VPAGE_SIZE;
        dword_t ppn_3 = sim.CreatePageTableLVL<false>(satp.fields.ppn, page_vaddr.GetVPN3());
        dword_t ppn_2 = sim.CreatePageTableLVL<false>(ppn_3, page_vaddr.GetVPN2());
        dword_t ppn_1 = sim.CreatePageTableLVL<false>(ppn_2, page_vaddr.GetVPN1());
        dword_t ppn_0 = sim.CreatePageTableLVL<true> (ppn_1, page_vaddr.GetVPN0(), PF_U | PF_R);

        (void)ppn_0;

        Exception exception = Exception::NONE;
        paddr_t paddr {0};

        std::tie(paddr, exception) = sim.hart_->mmu_.VirtToPhysAddr(page_vaddr, PF_X, *csr_regs, memory);
        ASSERT_EQ(exception, Exception::MMU_PAGE_ACCESS_EXECUTE);
    }

    {
        auto memory = PhysMemoryCtl();
        auto hart = Hart(&memory);
        auto sim = Simulator(&hart, &memory);

        sim.hart_->SetGPR(0x2, Simulator::STACK_PTR);
        CSRs *csr_regs = &(sim.hart_->csr_regs);

        reg_t root_page_idx = sim.AllocRootPageTable();
        sim.InitializeCSR(csr_regs, root_page_idx);

        auto satp = csr_regs->LoadCSR<CSRs::satp_t>(CSRs::Index::SATP);

        vaddr_t page_vaddr = Simulator::STACK_PTR - VPAGE_SIZE;
        dword_t ppn_3 = sim.CreatePageTableLVL<false>(satp.fields.ppn, page_vaddr.GetVPN3());
        dword_t ppn_2 = sim.CreatePageTableLVL<false>(ppn_3, page_vaddr.GetVPN2());
        dword_t ppn_1 = sim.CreatePageTableLVL<false>(ppn_2, page_vaddr.GetVPN1());
        dword_t ppn_0 = sim.CreatePageTableLVL<true> (ppn_1, page_vaddr.GetVPN0(), PF_U | PF_X);

        (void)ppn_0;

        Exception exception = Exception::NONE;
        paddr_t paddr {0};

        std::tie(paddr, exception) = sim.hart_->mmu_.VirtToPhysAddr(page_vaddr, PF_R, *csr_regs, memory);
        ASSERT_EQ(exception, Exception::NONE); // Executable is readable!
    }

    {
        auto memory = PhysMemoryCtl();
        auto hart = Hart(&memory);
        auto sim = Simulator(&hart, &memory);

        sim.hart_->SetGPR(0x2, Simulator::STACK_PTR);
        CSRs *csr_regs = &(sim.hart_->csr_regs);

        reg_t root_page_idx = sim.AllocRootPageTable();
        sim.InitializeCSR(csr_regs, root_page_idx);

        auto satp = csr_regs->LoadCSR<CSRs::satp_t>(CSRs::Index::SATP);

        vaddr_t page_vaddr = Simulator::STACK_PTR - VPAGE_SIZE;
        dword_t ppn_3 = sim.CreatePageTableLVL<false>(satp.fields.ppn, page_vaddr.GetVPN3());
        dword_t ppn_2 = sim.CreatePageTableLVL<false>(ppn_3, page_vaddr.GetVPN2());
        dword_t ppn_1 = sim.CreatePageTableLVL<false>(ppn_2, page_vaddr.GetVPN1());

        (void)ppn_1;

        Exception exception = Exception::NONE;
        paddr_t paddr {0};

        std::tie(paddr, exception) = sim.hart_->mmu_.VirtToPhysAddr(page_vaddr, PF_R, *csr_regs, memory);
        ASSERT_EQ(exception, Exception::MMU_INVALID_PAGE_ENTRY);
    }
}

} // namespace rvsim
