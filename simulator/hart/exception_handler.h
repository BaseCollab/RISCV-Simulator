#ifndef SIMULATOR_EXCEPTION_HANDLER_EXCEPTION_HANDLER_H
#define SIMULATOR_EXCEPTION_HANDLER_EXCEPTION_HANDLER_H

#include "common/macros.h"
#include "memory/memory_controller.h"
#include "csr.h"
#include "vpt.h"

namespace rvsim {

class ExceptionHandler {
public:
    static void MMUExceptionHandler(Hart *hart, MMU::Exception exception);

private:
    void VirtualPageMapping(vaddr_t vaddr, uint8_t rwx_flags);
};

} // namespace rvsim

#endif // SIMULATOR_EXCEPTION_HANDLER_EXCEPTION_HANDLER_H
