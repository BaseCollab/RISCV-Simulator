#include "supervisor/supervisor.h"
#include "hart/hart.h"
#include "memory/memory_controller.h"

namespace rvsim {

int Main(int argc, char *argv[])
{
    // Hart hart;
    // MemoryCtl memory;
    // Supervisor supervisor(&hart, &memory);

    (void)argc;
    (void)argv;

    return 0;
}

} // namespace rvsim

int main(int argc, char *argv[])
{
    return rvsim::Main(argc, argv);
}
