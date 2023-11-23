#ifndef SIMULATOR_MMU_TLB_H
#define SIMULATOR_MMU_TLB_H

#include "common/macros.h"
#include "common/constants.h"
#include "common/utils/cache.h"
#include "hart/vpt.h"

namespace rvsim {

template <size_t SIZE_LOG_2>
class TLB {
public:
    static constexpr size_t SIZE = 1 << SIZE_LOG_2;

    struct Data {
        byte_t *host_addr = nullptr;
        paddr_t paddr = 0x0;
    };

    using Cache = utils::DirectMappedCache<Data, vaddr_t, SIZE>;

public:
    NO_COPY_SEMANTIC(TLB);
    NO_MOVE_SEMANTIC(TLB);

    // virtual address 0x1 is invalid
    TLB() : cache_(0x1, Data {}) {}

    ~TLB() = default;

    const Data *LookUp(vaddr_t vaddr) const
    {
        return cache_.LookUp(vaddr >> vaddr_t::n_offset_bits, vaddr);
    }

    void Update(Data data, vaddr_t vaddr)
    {
        cache_.Update(data, vaddr >> vaddr_t::n_offset_bits, vaddr);
    }

private:
    Cache cache_;
};

} // namespace rvsim

#endif // SIMULATOR_MMU_TLB_H
