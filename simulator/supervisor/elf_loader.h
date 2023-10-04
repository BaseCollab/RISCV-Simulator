#ifndef SIMULATOR_ELF_LOADER_H
#define SIMULATOR_ELF_LOADER_H

#include "hart/hart.h"
#include "common/macros.h"
#include "common/constants.h"
#include "mmu/mmu.h"
#include "memory/memory_controller.h"

#include <libelf.h>
#include <gelf.h>

#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <sysexits.h>

#include <string>
#include <cstdio>
#include <optional>
#include <memory>

namespace rvsim {

// Loads segments into virtual memory
class ElfLoader {
public:
    NO_COPY_SEMANTIC(ElfLoader);
    NO_MOVE_SEMANTIC(ElfLoader);

    explicit ElfLoader(const std::string &elf_pathname);
    ~ElfLoader();

    void LoadElf(PhysMemoryCtl *memory_ctl, const Hart &hart);

private:
    void ValidateElfKind() const;

    static std::string_view GetElfKindString(Elf_Kind elf_kind);

private:
    std::string elf_pathname_;

    int elf_fd_ {0};
    Elf *elf_ {nullptr};
};

} // namespace rvsim

#endif // SIMULATOR_ELF_LOADER_H
