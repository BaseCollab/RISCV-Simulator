#include "supervisor/supervisor.h"
#include "hart/hart.h"
#include "memory/memory_controller.h"
#include "elf_loader/elf_loader.h"

namespace rvsim {

int Main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    auto memory = MemoryCtl();
    auto hart = Hart(&memory);
    auto supervisor = Supervisor(&hart, &memory);

    const char *elf_file = nullptr;
    supervisor.LoadElfFile(elf_file);

    return 0;
}

} // namespace rvsim

int main(int argc, char *argv[])
{
    return rvsim::Main(argc, argv);
}
