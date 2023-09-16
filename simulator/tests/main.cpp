#include <gtest/gtest.h>

// Just simple example (TO BE DELETED)
TEST(TestGroupName, Subtest_1)
{
    ASSERT_TRUE(1 == 1);
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
