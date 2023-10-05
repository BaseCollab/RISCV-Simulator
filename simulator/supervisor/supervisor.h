#ifndef SIMULATOR_SUPERVISOR_SUPERVISOR_H
#define SIMULATOR_SUPERVISOR_SUPERVISOR_H

#include "vpt.h"
#include "hart/hart.h"
#include "common/macros.h"
#include "common/constants.h"
#include "common/config.h"
#include "hart/hart.h"
#include "memory/memory_controller.h"
#include "elf_loader.h"

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

    explicit Supervisor(Hart *hart, PhysMemoryCtl *memory);
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
    void InitializeCSR(CSRs *csr_regs, reg_t root_page_idx);

    reg_t AllocRootPageTable();

    void SetExceptionHandlers();

    void MMUExceptionHandler(MMU::Exception exception);

private:
    Hart *hart_ {nullptr};

    PhysMemoryCtl *memory_ {nullptr};

    static ExceptionHandlers handlers_;
};

} // namespace rvsim

#endif // SIMULATOR_SUPERVISOR_SUPERVISOR_H
