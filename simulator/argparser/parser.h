#ifndef SIMULATOR_ARGPARSER_PARSER_H
#define SIMULATOR_ARGPARSER_PARSER_H

#include "common/constants.h"
#include "common/macros.h"

#include <getopt.h>

#include <string>
#include <vector>

namespace rvsim {

class ArgParser {
public:
    // clang-format off
    enum class SimModes : int {
        MODE_INVALID = -1,
        MODE_LLVM    = 0,
        MODE_JIT     = 1
    };

    enum class OptNames : int {
        OPT_EXFILE  = 1,
        OPT_OUTFILE = 'o',
        OPT_REGIME  = 'r',
        OPT_HELP    = 'h',
        OPT_PLUGIN  = 'p'
    };
    // clang-format on
public:
    NO_COPY_SEMANTIC(ArgParser);
    NO_MOVE_SEMANTIC(ArgParser);

    explicit ArgParser(int argc, char *argv[]) : argc_(argc), argv_(argv) {};
    ~ArgParser() = default;

    bool Parse();

    SimModes GetMode() const
    {
        return mode;
    }

    const std::string &GetElfFileName() const
    {
        return elf_filename;
    }

    const std::string &GetOutFn() const
    {
        return out_filename;
    }

    const std::vector<std::string> &GetPlugins() const
    {
        return plugins;
    }

private:
    void PrintHelp();

private:
    int argc_ {0};
    char **argv_ {nullptr};
    SimModes mode {SimModes::MODE_INVALID};

    /* All above filled by Parse() method */
    std::string elf_filename;
    std::string out_filename;
    std::vector<std::string> plugins;
};

} // namespace rvsim

#endif // SIMULATOR_ARGPARSER_PARSER_H
