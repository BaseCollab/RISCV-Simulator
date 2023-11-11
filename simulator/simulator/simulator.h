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

namespace rvsim {

class Simulator {
public:
    NO_COPY_SEMANTIC(Simulator);
    NO_MOVE_SEMANTIC(Simulator);

    explicit Simulator(Hart *hart, PhysMemoryCtl *memory);
    ~Simulator() = default;

    void LoadElfFile(const std::string &elf_pathname);
    void CreatePageTableWalk(vaddr_t vaddr) const;

private:
    void InitializeCSR(CSRs *csr_regs, reg_t root_page_idx);

    reg_t AllocRootPageTable();

    void SetExceptionHandlers();

    void PreparePageTable();

    template <bool IsLastLevel>
    dword_t CreatePageTableLVL(dword_t ppn_lvl, dword_t vpn, uint8_t rwx_flags = PF_R | PF_W) const;

private:
    static constexpr reg_t STACK_PTR = 0x600000000;

    Hart *hart_ {nullptr};

    PhysMemoryCtl *memory_ {nullptr};
};

} // namespace rvsim

#endif // SIMULATOR_SIMULATOR_SIMULATOR_H
