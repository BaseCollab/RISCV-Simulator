#ifndef SIMULATOR_EXCEPTION_HANDLER_EXCEPTION_HANDLER_H
#define SIMULATOR_EXCEPTION_HANDLER_EXCEPTION_HANDLER_H

#include "common/macros.h"
#include "mmu/mmu.h"
#include "memory/memory_controller.h"
#include "hart.h"
#include "csr.h"
#include "vpt.h"

namespace rvsim {

class ExceptionHandler {
public:
    static void MMUExceptionHandler(Hart *hart, PhysMemoryCtl *memory, Exception exception, addr_t vaddr,
                                    uint8_t rwx_flags);
};

} // namespace rvsim

#endif // SIMULATOR_EXCEPTION_HANDLER_EXCEPTION_HANDLER_H
