#include <gtest/gtest.h>
#include <memory>

#include "common/utils/stack.h"

namespace rvsim {

class StackTest : public testing::Test {
public:
    static constexpr size_t STACK_SIZE = 4096;

    void SetUp() override
    {
        stack_ = std::make_unique<utils::Stack<size_t>>(STACK_SIZE);
    }

    void TearDown() override {}

protected:
    std::unique_ptr<utils::Stack<size_t>> stack_;
};

TEST_F(StackTest, PushAndGetSize)
{
    ASSERT_EQ(stack_->Empty(), true);

    for (size_t i = 0; i < StackTest::STACK_SIZE; ++i) {
        stack_->Push(i);
    }
    ASSERT_EQ(stack_->GetSize(), StackTest::STACK_SIZE);

    size_t iteration_nmb = 10000;
    for (size_t i = 0; i < iteration_nmb; ++i) {
        auto error = stack_->Push(i);
        ASSERT_EQ(error.value(), utils::Stack<size_t>::Error::OVERFLOW);
    }
    // stack has static size
    ASSERT_EQ(stack_->GetSize(), StackTest::STACK_SIZE);
}

TEST_F(StackTest, PushAndTop)
{
    ASSERT_EQ(stack_->Empty(), true);

    for (size_t i = 0; i < StackTest::STACK_SIZE; ++i) {
        stack_->Push(i);
        ASSERT_EQ(stack_->Top().first, i);
    }
    ASSERT_EQ(stack_->GetSize(), StackTest::STACK_SIZE);
}

TEST_F(StackTest, PushAndPop)
{
    ASSERT_EQ(stack_->Empty(), true);

    for (size_t i = 1; i <= StackTest::STACK_SIZE; ++i) {
        stack_->Push(i);
    }
    ASSERT_EQ(stack_->GetSize(), StackTest::STACK_SIZE);

    ASSERT_EQ(stack_->Top().first, StackTest::STACK_SIZE);

    for (size_t i = StackTest::STACK_SIZE; i > 0; --i) {
        stack_->Pop();
        ASSERT_EQ(stack_->GetSize(), i - 1);
        ASSERT_EQ(stack_->Top().first, i - 1);
    }
}

} // namespace rvsim
