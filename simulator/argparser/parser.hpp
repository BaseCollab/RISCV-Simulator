#ifndef SIMULATOR_ARGPARSER_PARSER_H
#define SIMULATOR_ARGPARSER_PARSER_H

#include "common/constants.h"
#include <getopt.h>
#include <cstddef>

#include <string>
#include <cstring>
#include <iostream>
#include <vector>

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
    OPT_PLUGIN = 'p'
};


class ArgParser {
public:

    ArgParser(int argc, char *argv[]);

    bool Parse();
    
    SIM_MODES GetMode() { return mode; }
    std::string& GetExecFn() { return exec_fn; }
    std::string& GetOutFn() { return out_fn; }
    std::vector<std::string>& GetPlugins() { return plugins; }

private:
    int argc_;
    char **argv_;
    SIM_MODES mode;

    void PrintHelp();

    /* All above filled by Parse() method */
    std::string exec_fn; 
    std::string out_fn;
    std::vector<std::string> plugins;


};

} // end of rvsim namespace

#endif 
