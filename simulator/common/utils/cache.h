#ifndef SIMULATOR_COMMON_UTILS_CACHE_H
#define SIMULATOR_COMMON_UTILS_CACHE_H

#include "common/macros.h"

#include <cstddef>

namespace rvsim::utils {

template <typename DataType, typename TagType, size_t Size>
class DirectMappedCache {
public:
    NO_COPY_SEMANTIC(DirectMappedCache);
    NO_MOVE_SEMANTIC(DirectMappedCache);

    explicit DirectMappedCache(TagType init_tag, DataType init_data)
    {
        for (size_t i = 0; i < Size; ++i) {
            cache_[i].tag = init_tag;
            cache_[i].data = init_data;
        }
    }

    ~DirectMappedCache() = default;

    const DataType *LookUp(size_t offset, TagType tag) const
    {
        if (cache_[offset % Size].tag == tag) {
            return &(cache_[offset % Size].data);
        } else {
            return nullptr;
        }
    }

    void Update(const DataType &data, size_t offset, TagType tag)
    {
        cache_[offset % Size].data = data;
        cache_[offset % Size].tag = tag;
    }

private:
    struct Entry {
        DataType data;
        TagType tag;
    };

private:
    Entry cache_[Size];
};

} // namespace rvsim::utils

#endif // SIMULATOR_COMMON_UTILS_CACHE_H
