#include "gtest/gtest.h"
#include "simple_diff.h"

TEST(longest_common_sub_sequence, test1)
{
    std::vector<std::byte> x = {std::byte{0}, std::byte{1}, std::byte{3}};
    std::vector<std::byte> y = {std::byte{0}, std::byte{2}, std::byte{1}};
    auto result = simple_diff::getLongestCommonSubSequence(x, y);
    ASSERT_EQ(result, std::vector<std::byte>({std::byte{0}, std::byte{1}}));
    ASSERT_EQ(result, simple_diff::getLongestCommonSubSequence(y, x));
}

TEST(longest_common_sub_sequence, test2)
{
    std::vector<std::byte> x = {std::byte{0}, std::byte{1}, std::byte{2}};
    std::vector<std::byte> y = {std::byte{3}, std::byte{4}, std::byte{5}};
    auto result = simple_diff::getLongestCommonSubSequence(x, y);
    ASSERT_EQ(result, std::vector<std::byte>());
    ASSERT_EQ(result, simple_diff::getLongestCommonSubSequence(y, x));
}

TEST(longest_common_sub_sequence, test3)
{
    std::vector<std::byte> x = {std::byte{0}, std::byte{1}, std::byte{2}, std::byte{2}, std::byte{1}, std::byte{2}, std::byte{1}};
    std::vector<std::byte> y = {std::byte{1}, std::byte{1}, std::byte{2}, std::byte{1}};
    auto result = simple_diff::getLongestCommonSubSequence(x, y);
    ASSERT_EQ(result, std::vector<std::byte>({std::byte{1}, std::byte{1}, std::byte{2}, std::byte{1}}));
    ASSERT_EQ(result, simple_diff::getLongestCommonSubSequence(y, x));
}

TEST(longest_common_sub_sequence, test4)
{
    std::vector<std::byte> x = {std::byte{0}, std::byte{1}, std::byte{2}, std::byte{2}, std::byte{1}, std::byte{2}, std::byte{1}};
    std::vector<std::byte> y = {};
    auto result = simple_diff::getLongestCommonSubSequence(x, y);
    ASSERT_EQ(result, std::vector<std::byte>());
    ASSERT_EQ(result, simple_diff::getLongestCommonSubSequence(y, x));
}
