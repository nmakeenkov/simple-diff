#include <stdexcept>
#include "simple_diff.h"

simple_diff::Diff simple_diff::computeDiff(const std::vector<std::byte>& source, const std::vector<std::byte>& target)
{
    auto longestCommonSubSequence = getLongestCommonSubSequence(source, target);

    std::vector<simple_diff::ChangeInstance> changes;
    size_t j = 0;
    size_t lcsIndex = 0;
    for (size_t i = 0; i < source.size(); ++i)
    {
        if (lcsIndex < longestCommonSubSequence.size() && source[i] == longestCommonSubSequence[lcsIndex])
        {
            if (j < target.size())
            {
                while (target[j] != longestCommonSubSequence[lcsIndex])
                {
                    changes.emplace_back(i, Change::ADD, target[j]);
                    ++j;
                }

                // for eq symbol
                ++j;
            }
            ++lcsIndex;

            continue;
        }
        if (j < target.size()
                && (lcsIndex == longestCommonSubSequence.size() || target[j] != longestCommonSubSequence[lcsIndex]))
        {
            changes.emplace_back(i, Change::REPLACE, target[j++]);
        }
        else
        {
            changes.emplace_back(i, Change::REMOVE, std::byte{0});
        }
    }

    while (j < target.size())
    {
        changes.emplace_back(source.size(), Change::ADD, target[j++]);
    }

    return simple_diff::Diff(source, changes);
}

std::vector<std::byte> simple_diff::Diff::computeResult() const
{
    std::vector<std::byte> result;
    auto change = changes_.cbegin();
    for (size_t i = 0; i < source_.size(); ++i)
    {
        bool shouldAdd = true;
        while (change != changes_.end() && change->index() == i)
        {
            switch (change->change())
            {
                case Change::ADD:
                    result.push_back(change->symbol());
                    break;
                case Change::REPLACE:
                    shouldAdd = false;
                    result.push_back(change->symbol());
                    break;
                case Change::REMOVE:
                    shouldAdd = false;
                    break;
                default:
                    throw std::runtime_error("invalid change");
            }
            ++change;
        }
        if (shouldAdd)
        {
            result.push_back(source_[i]);
        }
    }

    while (change != changes_.end())
    {
        if (change->change() != Change::ADD)
        {
            throw std::runtime_error("illegal state");
        }
        result.push_back(change->symbol());
        ++change;
    }

    return result;
}
