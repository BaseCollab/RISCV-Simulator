#ifndef SIMULATOR_COMMON_UTILS_CACHE_H
#define SIMULATOR_COMMON_UTILS_CACHE_H

#include "common/macros.h"

#include <cstddef>
#include <optional>
#include <utility>
#include <cassert>

namespace rvsim::utils {

template <typename DataType, typename TagType, size_t Size>
class DirectMappedCache {
public:
    enum class Error {
        NONE = -1,
        LOOKUP = 1,
    };

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

    std::pair<const DataType *, Error> LookUp(size_t offset, TagType tag);
    {
        if (cache_[offset % Size].tag == tag) {
            return std::make_pair(&(cache_[offset % Size].data), Error::NONE);
        }
        else {
            return std::make_pair(nullptr, Error::LOOKUP);
        }
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
