#pragma once

#include <cstddef>
#include <vector>
#include <string>

namespace simple_diff
{
// Method is public to be testable
std::vector<std::byte> getLongestCommonSubSequence(const std::vector<std::byte>& first,
        const std::vector<std::byte>& second) noexcept;

enum class Change
{
    ADD,
    REMOVE,
    REPLACE
};

class ChangeInstance
{
private:
    size_t index_;
    Change change_;
    std::byte symbol_;

public:
    ChangeInstance() = default;
    ChangeInstance(size_t index, Change change, std::byte symbol) : index_(index), change_(change), symbol_(symbol) {}

    size_t index() const
    {
        return index_;
    }

    Change change() const
    {
        return change_;
    }

    std::byte symbol() const
    {
        return symbol_;
    }
};

class Diff
{
private:
    std::vector<std::byte> source_;
    std::vector<ChangeInstance> changes_;

public:
    Diff(const std::vector<std::byte>& source, const std::vector<ChangeInstance>& changes) :
            source_(source),changes_(changes) {}

    std::vector<std::byte> computeResult() const;

    std::string toHumanReadable() const;
};

Diff diffFromHumanReadableString(const std::vector<std::byte>& source, const std::string& string);

Diff computeDiff(const std::vector<std::byte>& source, const std::vector<std::byte>& target);

}
