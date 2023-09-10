#ifndef SIMULATOR_SUPERVISOR_SUPERVISOR_H
#define SIMULATOR_SUPERVISOR_SUPERVISOR_H

#include "macros.h"
#include "constants.h"
#include "hart.h"
#include "vpt.h"

namespace rvsim {

class Supervisor {
private:
    static constexpr reg_t root_page_number_ = 0;

public:
    NO_COPY_SEMANTIC(Supervisor);
    NO_MOVE_SEMANTIC(Supervisor);

    Supervisor(Hart &hart);
    ~Supervisor() = default;
};

} // rvsim

#endif // SIMULATOR_SUPERVISOR_SUPERVISOR_H
