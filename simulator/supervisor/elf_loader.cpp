#include "elf_loader.h"
#include "supervisor.h"

#include <iostream>

namespace rvsim {

ElfLoader::ElfLoader(const std::string &elf_pathname) : elf_pathname_(elf_pathname)
{
    if (elf_version(EV_CURRENT) == EV_NONE) {
        errx(EX_DATAERR, "ELF library initialization failed: %s ", elf_errmsg(-1));
    }

    if (elf_pathname_.empty()) {
        std::cerr << "ElfLoader error: input elf file name is empty" << std::endl;
        return;
    }

    elf_fd_ = open(elf_pathname_.c_str(), O_RDONLY);
    if (elf_fd_ == -1) {
        err(EX_NOINPUT, "Open \"%s\" failed: %s", elf_pathname_.c_str(), strerror(errno));
    }

    elf_ = elf_begin(elf_fd_, ELF_C_READ, nullptr);
    if (elf_ == nullptr) {
        errx(EX_SOFTWARE, "elf_begin() failed : %s. ", elf_errmsg(-1));
    }

    ValidateElfKind();
}

ElfLoader::~ElfLoader()
{
    elf_end(elf_);
    close(elf_fd_);
}

void ElfLoader::LoadElf(PhysMemoryCtl *memory_ctl, const Hart &hart)
{
    GElf_Ehdr elf_header;
    if (gelf_getehdr(elf_, &elf_header) == nullptr) {
        errx(EX_DATAERR, "getehdr() failed : %s. ", elf_errmsg(-1));
    }

    int elf_class = gelf_getclass(elf_);
    if (elf_class == ELFCLASSNONE) {
        errx(EX_DATAERR, "getclass() failed: %s. ", elf_errmsg(-1));
    } else if (elf_class == ELFCLASS32) {
        std::cerr << "ElfLoader error: the file is 32 bit, 64 bit is required" << std::endl;
    }

    Elf64_Half segments_count = elf_header.e_phnum;

    for (size_t i = 0; i < segments_count; ++i) {
        // Get next header
        GElf_Phdr *curr_segment_header = gelf_getphdr(elf_, i, nullptr);
        if (curr_segment_header == nullptr) {
            errx(EX_DATAERR, "gelf_getphdr() result is nullptr: %s. ", elf_errmsg(-1));
        }

        if (curr_segment_header->p_type == PT_LOAD) {
            // if curr_segment_header is loadable program segment then load it to virutal memory
            Elf64_Xword segment_vaddr = curr_segment_header->p_vaddr;
            Elf64_Xword segment_size = curr_segment_header->p_memsz;
            Elf64_Word segment_flags = curr_segment_header->p_flags;

            // exception
            // Supervisor::GetExceptionHandlers().mmu_handler({ MMU::Exception::INVALID_PAGE_ENTRY, segment_vaddr,
            // segment_flags });
            (void)segment_flags;

            auto addr_err_pair =
                hart.GetMMU().VirtToPhysAddr(segment_vaddr, MMU::Target::READ, hart.csr_regs, *memory_ctl);

            paddr_t phys_addr = addr_err_pair.first;
            memory_ctl->Store(phys_addr.value, curr_segment_header, segment_size);
        }
    }
}

void ElfLoader::ValidateElfKind() const
{
    auto elf_kd = ElfLoader::GetElfKindString(elf_kind(elf_));
    if (elf_kd != "elf object") {
        errx(EX_DATAERR, "\"%s\" is not an ELF object, type of file is %s", elf_pathname_.c_str(), elf_kd.data());
    }
}

/* static */
std::string_view ElfLoader::GetElfKindString(Elf_Kind elf_kind)
{
    switch (elf_kind) {
        case ELF_K_AR:
            return "ar archive";

        case ELF_K_COFF:
            return "COFF object";

        case ELF_K_ELF:
            return "elf object";

        case ELF_K_NONE:
            return "unknown";

        default:
            return "";
    }
}

} // namespace rvsim
