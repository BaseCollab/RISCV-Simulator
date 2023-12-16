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

        for (size_t i = 1; i <= sim.n_stack_pages_; ++i) {
            sim.MapVirtualPage(Simulator::STACK_PTR - VPAGE_SIZE * i);
        }

        Exception exception = Exception::NONE;
        paddr_t paddr {0};

        std::tie(paddr, exception) = sim.hart_->mmu_.VirtToPhysAddr(Simulator::STACK_PTR, PF_W, *csr_regs, memory);
        ASSERT_EQ(exception, Exception::MMU_INVALID_PAGE_ENTRY);
    }
}

} // namespace rvsim
