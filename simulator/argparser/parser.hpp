#ifndef SIMULATOR_ARGPARSER_PARSER_H
#define SIMULATOR_ARGPARSER_PARSER_H

#include "common/constants.h"
#include <getopt.h>
#include <cstddef>

#include <string>
#include <cstring>
#include <iostream>

namespace rvsim {

enum SIM_MODES {
    MODE_INVALID     = -1,
    MODE_LLVM       = 0,    /*  */
    MODE_2          = 1,    /* to be discussed */
};

enum OPT_NAMES {
    OPT_EXFILE = 1,
    OPT_OUTFILE = 'o',
    OPT_REGIME = 'r',
    OPT_HELP = 'h',
};


class ArgParser {
public:
    std::string exec_fn;
    std::string out_fn;

    ArgParser(int argc, char *argv[]);
    bool Parse();
    SIM_MODES GetMode() { return mode; }

private:
    int argc_;
    char **argv_;
    SIM_MODES mode;

    void PrintHelp();
};

} // end of rvsim namespace

#endif 
