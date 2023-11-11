#include "simulator/elf_loader.h"
#include "simulator/simulator.h"

#include <unistd.h>
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

    elf_buffer_size_ = lseek(elf_fd_, 0, SEEK_END);
    lseek(elf_fd_, 0, SEEK_SET);

#ifdef DEBUG_ELF
    std::cerr << "[DEBUG] [ELF] Elf file size is = " << elf_buffer_size_ << std::endl;
#endif

    elf_buffer_ = new uint8_t[elf_buffer_size_ * sizeof(uint8_t)];
    if (elf_buffer_ == nullptr) {
        err(EX_OSERR, "File \"%s\" buffering failed (new)", elf_pathname_.c_str());
    }

    int read_error = read(elf_fd_, elf_buffer_, elf_buffer_size_);
    if (read_error == -1) {
        err(EX_OSERR, "Read of \"%s\" failed (read()): %s", elf_pathname_.c_str(), strerror(errno));
    }

    elf_ = elf_begin(elf_fd_, ELF_C_READ, nullptr);
    if (elf_ == nullptr) {
        errx(EX_SOFTWARE, "elf_begin() failed : %s. ", elf_errmsg(-1));
    }

    ValidateElfKind();
}

ElfLoader::~ElfLoader()
{
    delete[] elf_buffer_;

    elf_end(elf_);
    close(elf_fd_);
}

void ElfLoader::LoadElf(const Simulator &sim, const Hart &hart)
{
    GElf_Ehdr elf_header;
    if (gelf_getehdr(elf_, &elf_header) == nullptr) {
        errx(EX_DATAERR, "getehdr() failed: %s.", elf_errmsg(-1));
    }

    int elf_class = gelf_getclass(elf_);
    if (elf_class == ELFCLASSNONE) {
        errx(EX_DATAERR, "getclass() failed: %s.", elf_errmsg(-1));
    } else if (elf_class == ELFCLASS32) {
        std::cerr << "ElfLoader error: the file is 32 bit, 64 bit is required" << std::endl;
        return;
    }

    elf_entry_ = elf_header.e_entry;

    Elf64_Half segments_count = elf_header.e_phnum;

#ifdef DEBUG_ELF
    std::cerr << "[DEBUG] [ELF] Amount of elf segments = " << segments_count << std::endl;
#endif

    for (size_t i = 0; i < segments_count; ++i) {
        // Get next header
        GElf_Phdr curr_segment_header;
        if (gelf_getphdr(elf_, i, &curr_segment_header) == nullptr) {
            errx(EX_DATAERR, "gelf_getphdr() result is nullptr: %s.", elf_errmsg(-1));
        }

        if (curr_segment_header.p_type == PT_LOAD) {
            // If curr_segment_header is loadable program segment then load it to virtual memory
            Elf64_Xword segment_vaddr = curr_segment_header.p_vaddr;
            Elf64_Xword segment_elf_size = curr_segment_header.p_filesz;
            Elf64_Xword segment_mem_size = curr_segment_header.p_memsz;
            Elf64_Off segment_file_offset = curr_segment_header.p_offset;
            Elf64_Word segment_flags = curr_segment_header.p_flags;

#ifdef DEBUG_ELF
            std::cerr << "[DEBUG] [ELF] Segment " << i << " is PT_LOAD:" << std::endl;
            std::cerr << "[DEBUG] [ELF]    p_vaddr = " << segment_vaddr << std::endl;
            std::cerr << "[DEBUG] [ELF]    p_filesz = " << segment_elf_size << std::endl;
            std::cerr << "[DEBUG] [ELF]    p_memsz = " << segment_mem_size << std::endl;
            std::cerr << "[DEBUG] [ELF]    p_offset = " << segment_file_offset << std::endl;
            std::cerr << "[DEBUG] [ELF]    p_flags = " << segment_flags << std::endl;
#endif

            sim.MapVirtualRange(segment_vaddr, segment_vaddr + segment_mem_size);
            hart.StoreToMemory(segment_vaddr, elf_buffer_ + segment_file_offset, segment_elf_size, segment_flags);
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
