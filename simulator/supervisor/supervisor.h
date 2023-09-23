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

private:
    static constexpr reg_t root_page_number_ = 0;

    void InitializeCSR(CSRs *csr_regs);
    int LocateRootPageTable(CSRs *csr_regs);

    void SetExceptionHandlers();

    void MMUExceptionHandler(MMU::Exception exception);

private:
    Hart *hart_ {nullptr};

    MemoryCtl *memory_ {nullptr};

    static ExceptionHandlers handlers_;
};

} // namespace rvsim

#endif // SIMULATOR_SUPERVISOR_SUPERVISOR_H
