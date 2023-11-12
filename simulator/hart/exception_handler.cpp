#include "exception_handler.h"

#include <iostream>
#include <bitset>
#include <cassert>
#include <elf.h>
#include <sysexits.h>
#include <err.h>

namespace rvsim {

void ExceptionHandler::MMUExceptionHandler(Hart *hart, PhysMemoryCtl *memory, Exception exception, addr_t vaddr)
{
    (void)hart;
    (void)memory;

    if (exception == Exception::NONE) {
        return;
    }

    switch (exception) {
        case Exception::MMU_INVALID_PAGE_ENTRY:
            errx(EX_SOFTWARE, "Page access fault: invalid page entry [addr %lx]", vaddr);

        case Exception::MMU_INVALID_PAGE_SIZE:
            errx(EX_SOFTWARE,
                "Superpages, megapages, etc - are not supported (rwx != 0x0 for not final page table entry) [addr %lx]",
                vaddr);

        case Exception::MMU_PAGE_WRITE_NO_READ:
            errx(EX_SOFTWARE, "Page invalid rights: write permission without read permission [addr %lx]", vaddr);

        case Exception::MMU_PAGE_ACCESS_READ:
            errx(EX_SOFTWARE, "Page access fault: read without read permission [addr %lx]", vaddr);

        case Exception::MMU_PAGE_ACCESS_WRITE:
            errx(EX_SOFTWARE, "Page access fault: write without write permission [addr %lx]", vaddr);

        case Exception::MMU_PAGE_ACCESS_EXECUTE:
            errx(EX_SOFTWARE, "Page access fault: execute without execute permission [addr %lx]", vaddr);

        case Exception::MMU_ADDRESS_MISALIGNED:
            errx(EX_SOFTWARE, "Misaligned address: [addr %lx]", vaddr);

        default:
            errx(EX_SOFTWARE, "Unknown exception type: [addr %lx]", vaddr);
    }
}

} // namespace rvsim
