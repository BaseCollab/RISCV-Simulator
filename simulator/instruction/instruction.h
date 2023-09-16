#ifndef SIMULATOR_INSTRUCTION_INSTRUCTION_H
#define SIMULATOR_INSTRUCTION_INSTRUCTION_H

#include "common/constants.h"
#include "common/macros.h"

#include <cstdint>

namespace rvsim {

class Instruction {
public:
    NO_COPY_SEMANTIC(Instruction);
    NO_MOVE_SEMANTIC(Instruction);

    Instruction() = default;
    ~Instruction() = default;

private:

};

} // namespace rvsim

#endif // SIMULATOR_INSTRUCTION_INSTRUCTION_H
