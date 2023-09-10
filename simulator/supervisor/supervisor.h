#ifndef SIMULATOR_SUPERVISOR_SUPERVISOR_H
#define SIMULATOR_SUPERVISOR_SUPERVISOR_H

#include "common/macros.h"
#include "common/constants.h"
#include "hart/hart.h"
#include "memory/memory_controller.h"
#include "vpt.h"

namespace rvsim {

class Supervisor {
private:
    static constexpr reg_t root_page_number_ = 0;

    void initializeCSR(CSRs *csr_regs);
    int locateRootPageTable(CSRs *csr_regs, MemoryCtl *memory);

public:
    NO_COPY_SEMANTIC(Supervisor);
    NO_MOVE_SEMANTIC(Supervisor);

    Supervisor(CSRs *csr_regs, MemoryCtl *memory);
    ~Supervisor() = default;
};

} // rvsim

#endif // SIMULATOR_SUPERVISOR_SUPERVISOR_H
