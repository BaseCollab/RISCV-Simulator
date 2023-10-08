#ifndef SIMULATOR_ARGPARSER_PARSER_H
#define SIMULATOR_ARGPARSER_PARSER_H

#include "common/constants.h"
#include <getopt.h>
#include <cstddef>

#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include "common/macros.h"

namespace rvsim {

enum class SimModes : int {
    MODE_INVALID = -1,
    MODE_LLVM = 0, /*  */
    MODE_2 = 1,    /* to be discussed */
};

enum class OptNames : int { OPT_EXFILE = 1, OPT_OUTFILE = 'o', OPT_REGIME = 'r', OPT_HELP = 'h', OPT_PLUGIN = 'p' };

class ArgParser {
public:
    NO_COPY_SEMANTIC(ArgParser);
    NO_MOVE_SEMANTIC(ArgParser);

    explicit ArgParser(int argc, char *argv[]);

    bool Parse();

    SimModes GetMode() const
    {
        return mode;
    }
    const std::string &GetExecFn() const
    {
        return elf_fn;
    }
    const std::string &GetOutFn() const
    {
        return out_fn;
    }
    const std::vector<std::string> &GetPlugins() const
    {
        return plugins;
    }

private:
    int argc_;
    char **argv_;
    SimModes mode;

    void PrintHelp();

    /* All above filled by Parse() method */
    std::string elf_fn;
    std::string out_fn;
    std::vector<std::string> plugins;
};

} // namespace rvsim

#endif
