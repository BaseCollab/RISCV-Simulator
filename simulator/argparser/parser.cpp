#include "parser.hpp"

namespace rvsim {

ArgParser::ArgParser(int argc, char *argv[])  :
    exec_fn(),
    out_fn(),
{
        argc_ = argc;
        argv_ = argv;
}

/*  print help message/get input file (exec_fn)/output file(out_fn) and return true.
    on the parse error: print help message and return false */
bool ArgParser::Parse()
{
    if (argc_ < 2) {
        printUsage(argv_[0]);
        return false;
    }

    static struct option long_options[] = {
            {"help",        no_argument,       0,  OPT_HELP },
            {"out_file",    required_argument, 0,  OPT_OUTFILE },
            {0, 0, 0, 0}
        };


    int opt = 0;
    while ((opt = getopt(argc, argv, "-o:h", long_options, NULL)) != -1) {
        switch (opt) {
        case OPT_FILE:
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
    printf("--[h]elp - print this help and exit\n");
}

} // end of rvsim namespace