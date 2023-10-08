#ifndef SIMULATOR_ARGPARSER_PARSER_H
#define SIMULATOR_ARGPARSER_PARSER_H

#include "common/constants.h"
#include <getopt.h>
#include <cstddef>

#include <string>

namespace rvsim {

enum SIM_MODES {
    PARSE_ERROR  = -1,
    MODE_1       = 0, /* to be discussed */
    MODE_2       = 1, /* to be discussed */
};

enum OPT_NAMES {
    OPT_FILE = 1,
    OPT_OUTFILE = 'o',
    OPT_HELP = 'h',
};


class ArgParser {
public:
    std::string exec_fn;
    std::string out_fn;

    ArgParser(int argc, char *argv[]);
    bool Parse();

private:
    int argc_;
    char **argv_;

    void PrintHelp();
};

} // end of rvsim namespace

#endif 
