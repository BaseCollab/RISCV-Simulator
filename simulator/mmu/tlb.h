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
        paddr_t paddr = 0x0;
    };

    using Tag = vaddr_t;

    using Cache = utils::DirectMappedCache<Data, Tag, TLB_SIZE>;
    class Error : Cache::Error {};

public:
    NO_COPY_SEMANTIC(TLB);
    NO_MOVE_SEMANTIC(TLB);

    TLB() = default;
    ~TLB() = default;

    std::pair<const Data *, Error> LookUp(size_t offset, Tag tag)
    {
        return cache_.LookUp(offset, tag);
    }

private:
    Cache cache_;
};

} // namespace rvsim

#endif // SIMULATOR_MMU_TLB_H
