#ifndef SIMULATOR_SIMULATOR_SIMULATOR_H
#define SIMULATOR_SIMULATOR_SIMULATOR_H

#include "common/macros.h"
#include "common/constants.h"
#include "common/config.h"
#include "hart/hart.h"
#include "hart/vpt.h"
#include "hart/exception_handler.h"
#include "memory/memory_controller.h"

#include <functional>
#include <gtest/gtest_prod.h>

namespace rvsim {

class Simulator {
public:
    static constexpr size_t DEFAULT_STACK_PAGES_NUM = 1 << 5;

public:
    NO_COPY_SEMANTIC(Simulator);
    NO_MOVE_SEMANTIC(Simulator);

    explicit Simulator(Hart *hart, PhysMemoryCtl *memory, size_t stack_pages = DEFAULT_STACK_PAGES_NUM);
    ~Simulator() = default;

    void LoadElfFile(const std::string &elf_pathname);

    void MapVirtualPage(vaddr_t page_vaddr, uint8_t urwx_flags = PF_U | PF_R | PF_W) const;
    void MapVirtualRange(vaddr_t vaddr_start, vaddr_t vaddr_end, uint8_t urwx_flags = PF_U | PF_R | PF_W) const;

    Hart *GetActiveHart();

private:
    void InitializeCSR(CSRs *csr_regs, reg_t root_page_idx);

    reg_t AllocRootPageTable();
    void SetExceptionHandlers();
    void PreparePageTable();

    template <bool IsLastLevel>
    dword_t CreatePageTableLVL(dword_t ppn_lvl, dword_t vpn, uint8_t urwx_flags = PF_U | PF_R | PF_W) const
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

private:
    static constexpr reg_t STACK_PTR = 0x600000000;
    size_t n_stack_pages_;

    Hart *hart_ {nullptr};

    PhysMemoryCtl *memory_ {nullptr};

private:
    FRIEND_TEST(MMUTest, ComplexTest);
};

} // namespace rvsim

#endif // SIMULATOR_SIMULATOR_SIMULATOR_H
