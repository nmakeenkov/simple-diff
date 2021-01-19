#include "simple_diff.h"
#include <vector>
#include <cstddef>
#include <algorithm>
#include <cassert>

namespace
{
enum class Previous
{
    ZERO,
    DECREMENT_FIRST,
    DECREMENT_SECOND,
    DECREMENT_BOTH
};

class LengthAndPrevious
{
private:
    size_t length_;
    Previous previous_;

public:
    LengthAndPrevious(size_t length, Previous previous) : length_(length), previous_(previous) {}

    size_t length() const
    {
        return length_;
    }

    void stepBack(size_t& first_index, size_t& second_index, bool& exit) const
    {
        if (previous_ == Previous::ZERO)
        {
            exit = true;
            return;
        }
        if (previous_ == Previous::DECREMENT_FIRST || previous_ == Previous::DECREMENT_BOTH)
        {
            --first_index;
        }
        if (previous_ == Previous::DECREMENT_SECOND || previous_ == Previous::DECREMENT_BOTH)
        {
            --second_index;
        }
    };
};
}

std::vector<std::byte> simple_diff::getLongestCommonSubSequence(const std::vector<std::byte>& first,
        const std::vector<std::byte>& second) noexcept
{
    if (first.empty() || second.empty())
    {
        return {};
    }

    std::vector<std::vector<LengthAndPrevious>> lcs(first.size(),
            std::vector<LengthAndPrevious>(second.size(), {0, Previous::ZERO}));
    for (size_t i = 0; i < first.size(); ++i)
    {
        for (size_t j = 0; j < second.size(); ++j)
        {
            if (first[i] == second[j])
            {
                if (i > 0 && j > 0)
                {
                    lcs[i][j] = {lcs[i - 1][j - 1].length() + 1, Previous::DECREMENT_BOTH};
                }
                else
                {
                    lcs[i][j] = {1, Previous::ZERO};
                }
            }
            else
            {
                if (i == 0) {
                    if (j == 0)
                    {
                        lcs[i][j] = {0, Previous::ZERO};
                    }
                    else
                    {
                        lcs[i][j] = {lcs[i][j - 1].length(), Previous::DECREMENT_SECOND};
                    }
                }
                else if (j == 0)
                {
                    lcs[i][j] = {lcs[i - 1][j].length(), Previous::DECREMENT_FIRST};
                }
                else if (lcs[i][j - 1].length() > lcs[i - 1][j].length())
                {
                    lcs[i][j] = {lcs[i][j - 1].length(), Previous ::DECREMENT_SECOND};
                }
                else
                {
                    lcs[i][j] = {lcs[i - 1][j].length(), Previous ::DECREMENT_FIRST};
                }
            }
        }
    }

    size_t i = first.size() - 1;
    size_t j = second.size() - 1;
    size_t expectedLength = lcs[i][j].length();
    std::vector<std::byte> result;
    result.reserve(expectedLength);
    bool exit = false;
    do
    {
        if (first[i] == second[j])
        {
            result.push_back(first[i]);
        }
        lcs[i][j].stepBack(i, j, exit);
    }
    while (!exit);

    assert(result.size() == expectedLength);
    std::reverse(result.begin(), result.end());

    return result;
}
