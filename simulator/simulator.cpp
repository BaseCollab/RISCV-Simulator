#include "hart/hart.h"
#include "supervisor/supervisor.h"
#include "memory/memory_controller.h"
#include "argparser/parser.hpp"

namespace rvsim {

int Main(int argc, char *argv[])
{
    (void)argc;

    auto memory = PhysMemoryCtl();
    auto hart = Hart(&memory);
    auto supervisor = Supervisor(&hart, &memory);
    auto parser = ArgParser(argc, argv);
    
    if (!parser.Parse()) {
        return EXIT_FAILURE;
    }
    
  
    const char *elf_file = argv[1];
    supervisor.LoadElfFile(elf_file);

    hart.Interpret();

    return 0;
}

} // namespace rvsim

int main(int argc, char *argv[])
{
    if (argc < 2)
        errx(EX_NOINPUT, "Error: 1 argument required (.elf filename)");

    return rvsim::Main(argc, argv);
}
