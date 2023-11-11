#include "hart/hart.h"
#include "simulator/simulator.h"
#include "memory/memory_controller.h"
#include "argparser/parser.h"

namespace rvsim {

int Main(int argc, char *argv[])
{
    auto parser = ArgParser(argc, argv);
    if (!parser.Parse()) {
        return EXIT_FAILURE;
    }

    auto memory = PhysMemoryCtl();
    auto hart = Hart(&memory);
    auto sim = Simulator(&hart, &memory);

    sim.LoadElfFile(parser.GetElfFileName());

    // hart.Interpret();

    return 0;
}

} // namespace rvsim

int main(int argc, char *argv[])
{
    return rvsim::Main(argc, argv);
}
