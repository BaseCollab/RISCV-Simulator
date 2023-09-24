#ifndef SIMULATOR_SUPERVISOR_SUPERVISOR_H
#define SIMULATOR_SUPERVISOR_SUPERVISOR_H

#include "vpt.h"
#include "hart/hart.h"
#include "common/macros.h"
#include "common/constants.h"
#include "common/config.h"
#include "hart/hart.h"
#include "memory/memory_controller.h"
#include "elf_loader/elf_loader.h"

#include <functional>

namespace rvsim {

class Supervisor {
public:
    struct ExceptionHandlers {
        std::function<void(MMU::Exception)> mmu_handler;
    };

public:
    NO_COPY_SEMANTIC(Supervisor);
    NO_MOVE_SEMANTIC(Supervisor);

    explicit Supervisor(Hart *hart, MemoryCtl *memory);
    ~Supervisor() = default;

    void LoadElfFile(const std::string &elf_pathname);

    static const ExceptionHandlers &GetExceptionHandlers()
    {
        return handlers_;
    }

    /*
    In case of MMU::Exception::INVALID_PAGE_ENTRY or PAGE_FAULT
    operation systam aka Supervisor need to handle this exception
    and take steps to highlight the table
    */
    void VirtualPageMapping(vaddr_t vaddr, uint8_t rwx_flags);

private:
    void InitializeCSR(CSRs *csr_regs);

    uint16_t AllocRootPageTable();

    void SetExceptionHandlers();

    void MMUExceptionHandler(MMU::Exception exception);

private:
    Hart *hart_ {nullptr};

    MemoryCtl *memory_ {nullptr};

    static ExceptionHandlers handlers_;
    // idx of page where root virtual page table is located
    uint16_t root_page_idx_ {0};
};

} // namespace rvsim

#endif // SIMULATOR_SUPERVISOR_SUPERVISOR_H
