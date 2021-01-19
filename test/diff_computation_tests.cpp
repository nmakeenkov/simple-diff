#include "gtest/gtest.h"
#include "simple_diff.h"

TEST(diff_computation, test1)
{
    std::vector<std::byte> x = {std::byte{0}, std::byte{1}, std::byte{3}};
    std::vector<std::byte> y = {std::byte{0}, std::byte{2}, std::byte{1}};
    auto result = simple_diff::computeDiff(x, y);
    ASSERT_EQ(result.computeResult(), y);
}

TEST(diff_computation, test2)
{
    std::vector<std::byte> x = {std::byte{0}, std::byte{1}, std::byte{2}};
    std::vector<std::byte> y = {std::byte{3}, std::byte{4}, std::byte{5}};
    auto result = simple_diff::computeDiff(x, y);
    ASSERT_EQ(result.computeResult(), y);
}

TEST(diff_computation, test3)
{
    std::vector<std::byte> x = {std::byte{0}, std::byte{1}, std::byte{2}, std::byte{2}, std::byte{1}, std::byte{2}, std::byte{1}};
    std::vector<std::byte> y = {std::byte{1}, std::byte{1}, std::byte{2}, std::byte{1}};
    auto result = simple_diff::computeDiff(x, y);
    ASSERT_EQ(result.computeResult(), y);
}

TEST(diff_computation, test4)
{
    std::vector<std::byte> x = {std::byte{0}, std::byte{1}, std::byte{2}, std::byte{2}, std::byte{1}, std::byte{2}, std::byte{1}};
    std::vector<std::byte> y = {};
    auto result = simple_diff::computeDiff(x, y);
    ASSERT_EQ(result.computeResult(), y);
}

TEST(diff_computation, test5)
{
    std::vector<std::byte> x = {};
    std::vector<std::byte> y = {std::byte{0}, std::byte{1}, std::byte{2}, std::byte{2}, std::byte{1}, std::byte{2}, std::byte{1}};
    auto result = simple_diff::computeDiff(x, y);
    ASSERT_EQ(result.computeResult(), y);
}

TEST(diff_computation, test_add_to_beginning)
{
    std::vector<std::byte> x = {std::byte{1}, std::byte{2}};
    std::vector<std::byte> y = {std::byte{3}, std::byte{1}, std::byte{2}};
    auto result = simple_diff::computeDiff(x, y);
    ASSERT_EQ(result.computeResult(), y);
}

TEST(diff_computation, test_add_to_end)
{
    std::vector<std::byte> x = {std::byte{1}, std::byte{2}};
    std::vector<std::byte> y = {std::byte{1}, std::byte{2}, std::byte{0}};
    auto result = simple_diff::computeDiff(x, y);
    ASSERT_EQ(result.computeResult(), y);
}

TEST(diff_computation, test_no_diff)
{
    std::vector<std::byte> x = {std::byte{1}, std::byte{2}};
    std::vector<std::byte> y = {std::byte{1}, std::byte{2}};
    auto result = simple_diff::computeDiff(x, y);
    ASSERT_EQ(result.computeResult(), y);
}
