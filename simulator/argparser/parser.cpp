#include "parser.h"

namespace rvsim {

ArgParser::ArgParser(int argc, char *argv[])
{
    argc_ = argc;
    argv_ = argv;
    mode  = SimModes::MODE_INVALID;
}

/*  Get [input file (elf_fn)/output file(out_fn)/sim modes] and return true on success.
    On the parse error: print help message and return false
    Simulator mode stored in 'mode' variable */
bool ArgParser::Parse()
{
    if (argc_ < 2) {
        PrintHelp();
        return false;
    }

    static struct option options[] = {
        {"help", no_argument, 0, static_cast<int>(OptNames::OPT_HELP)}, {"regime", required_argument, 0, static_cast<int>(OptNames::OPT_REGIME)}, {0, 0, 0, 0}};

    int opt = 0;
    while ((opt = getopt_long(argc_, argv_, "-o:p:r:h", options, NULL)) != -1) {
        switch (OptNames(opt)) {
            case OptNames::OPT_REGIME:
                if (!strcmp(optarg, "llvm")) {
                    mode = SimModes::MODE_LLVM;
                } else {
                    return false;
                }
                break;
            case OptNames::OPT_PLUGIN:
                plugins.push_back(std::string(optarg));
                break;
            case OptNames::OPT_EXFILE:
                elf_fn = optarg;
                break;
            case OptNames::OPT_OUTFILE:
                out_fn = optarg;
                break;
            case OptNames::OPT_HELP:
                PrintHelp();
                exit(EXIT_SUCCESS);
            default:
                return false;
        }
    }

    return true;
}

void ArgParser::PrintHelp()
{
    printf("USAGE: %s <elf_fn> [options]\n\tSimulator options:\n", argv_[0]);
    printf("-o <out_fn> -- output file.\n");
    printf("-h          -- print this help and exit.\n");
    printf("-r <regime> -- for simulator regimes (ex: llvm).\n");
    printf("-p <*.so>   -- path to library to be used as plugin.\n");
}

} // namespace rvsim