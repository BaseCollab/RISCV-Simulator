#ifndef SIMULATOR_HART_EXCEPTION_H
#define SIMULATOR_HART_EXCEPTION_H

namespace rvsim {

// clang-format off
enum class Exception {
    NONE                            = -1,

    // MMU permission exceptions
    MMU_INVALID_PAGE_ENTRY          = 0x01,
    MMU_INVALID_PAGE_SIZE           = 0x02, // not all types of memory are supported (such as superpages)
    MMU_PAGE_WRITE_NO_READ          = 0x03,
    MMU_PAGE_ACCESS_READ            = 0x04,
    MMU_PAGE_ACCESS_WRITE           = 0x05,
    MMU_PAGE_ACCESS_EXECUTE         = 0x06,
    MMU_PAGE_SUPERVISOR_USER_ACCESS = 0x07,

    // Load/store exceptions
    MMU_ADDRESS_MISALIGNED          = 0x10,

    // Decode exceptions
    INVALID_INSTRUCTION_TYPE        = 0x20,
};
// clang-format on

} // namespace rvsim

#endif // SIMULATOR_HART_EXCEPTION_H
