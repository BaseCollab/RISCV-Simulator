#ifndef SIMULATOR_MMU_TLB_H
#define SIMULATOR_MMU_TLB_H

#include "common/macros.h"
#include "common/constants.h"
#include "common/utils/cache.h"
#include "hart/vpt.h"

namespace rvsim {

class TLB {
public:
    static constexpr size_t TLB_SIZE = 1 << 6;

    struct Data {
        void *host_addr = nullptr;
        vaddr_t vaddr_t {0x1};
        paddr_t paddr_t {0x0};
    }

public:
    NO_COPY_SEMANTIC(TLB);
    NO_MOVE_SEMANTIC(TLB);

    TLB() = default;
    ~TLB() = default;



private:
    utils::DirectMappedCache<Data, vaddr_t, TLB_SIZE>;
};

} // namespace rvsim

#endif // SIMULATOR_MMU_TLB_H
