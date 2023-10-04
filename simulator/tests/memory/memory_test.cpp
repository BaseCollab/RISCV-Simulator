#include <gtest/gtest.h>

#include <array>

#include "memory/memory_controller.h"

namespace rvsim {

TEST(PhysMemoryTest, StoreAndLoad)
{
    PhysMemory memory;

    std::array<int, 5> array = {0, 1, 2, 3, 4};
    std::array<int, 5> recv_array;

    memory.Store(0, array.data(), array.size() * sizeof(int));
    memory.Load(recv_array.data(), recv_array.size() * sizeof(int), 0);

    ASSERT_EQ(array, recv_array);
}

} // namespace rvsim
