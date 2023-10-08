#include "parser.hpp"

namespace rvsim {

ArgParser::ArgParser(int argc, char *argv[])
{
    argc_ = argc;
    argv_ = argv;
    mode = MODE_INVALID;
}

/*  Get [input file (exec_fn)/output file(out_fn)/sim modes] and return true on success.
    On the parse error: print help message and return false
    Simulator mode stored in 'mode' variable */
bool ArgParser::Parse()
{
    if (argc_ < 2) {
        PrintHelp();
        return false;
    }

    static struct option options[] = {
            {"help",        no_argument,       0,  OPT_HELP },
            {"regime",      required_argument, 0,  OPT_REGIME },
            {0, 0, 0, 0}
        };


    int opt = 0;
    while ((opt = getopt_long(argc_, argv_, "-o:r:h", options, NULL)) != -1) {
        switch (opt) {
        case OPT_REGIME:
            if (!strcmp(optarg, "llvm")) {
                mode = MODE_LLVM;
            }
            else {
                mode = MODE_INVALID;
                return false;
            }
            break;
        case OPT_EXFILE:
            exec_fn = optarg;
            break;
        case OPT_OUTFILE:
            out_fn = optarg;
            break;
        case OPT_HELP:
            PrintHelp();
            exit(EXIT_SUCCESS);
        default:
            return false;
        }
    }

    return true;
}

void ArgParser::PrintHelp() {
    printf("USAGE: %s <file> [options]\nRISC-V Options:\n", argv_[0]);
    printf("-o <file> - output file\n");
    printf("--h - print this help and exit\n");
    printf("--r <regime> -- for simulator regimes (ex: llvm)\n");
}

} // end of rvsim namespace