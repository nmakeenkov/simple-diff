#include "simple_diff.h"
#include <sstream>
#include <map>
#include <iomanip>
#include <cassert>

namespace
{
const std::string EMPTY_HEX = "    ";

std::byte readHex(const std::string& s)
{
    static std::map<std::string, std::byte> cache;

    auto it = cache.find(s);
    if (it != cache.end())
    {
        return it->second;
    }

    std::istringstream ss(s);
    int intValue;
    ss >> std::hex >> intValue;
    auto result = static_cast<std::byte>(intValue);

    cache[s] = result;
    return result;
}

std::string getHexRepresentation(std::byte byte)
{
    static std::array<std::string, 1 + static_cast<size_t>(std::numeric_limits<uint8_t>::max())> cache;

    if (!cache[static_cast<int>(byte)].empty())
    {
        return cache[static_cast<int>(byte)];
    }

    std::ostringstream ss;
    ss << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex << std::to_integer<int>(byte);
    auto result = ss.str();

    cache[static_cast<int>(byte)] = result;
    return result;
}

size_t getMaxShownIndex(const simple_diff::ChangeInstance& change)
{
    return change.change() == simple_diff::Change::ADD ? change.index() : change.index() + 1;
}

size_t getMinShownIndex(const simple_diff::ChangeInstance& change)
{
    return change.index() == 0 ? 0 : change.index() - 1;
}

size_t readIndex(const std::string& line)
{
    if (line.substr(0, 3) != "@@ ")
    {
        throw std::runtime_error("invalid diff line: \"" + line + "\"");
    }
    size_t commaIndex = line.find_first_of(',');

    return std::stoull(line.substr(3, commaIndex - 3));
}

/**
 * Get byte from serialized line
 *
 * @param first if the byte belongs to the first string
 * @param allowOther if it is allowed the placeholder for other string to contain characters
 */
std::byte getByte(const std::string& line, bool first, bool allowOther)
{
    // e.g. "+ 0x00 0x00"
    if (line.length() != 1 + 1 + 4 + 1 + 4 || line[1] != ' ' || line[1 + 4 + 1] != ' '
            || (!allowOther && line.substr(first ? 2 + 4 + 1 : 2, 4) != EMPTY_HEX))
    {
        throw std::runtime_error("invalid diff line: \"" + line + "\"");
    }
    return readHex(line.substr(first ? 2 : 2 + 4 + 1, 4));
}
}

std::string simple_diff::Diff::toHumanReadable() const
{
    std::string result;
    size_t i = 0;
    int targetIndexOffset = 0;
    while (i < changes_.size())
    {
        size_t j = i + 1;
        while (j < changes_.size() && getMaxShownIndex(changes_[j - 1]) + 2 >= getMinShownIndex(changes_[j]))
        {
            ++j;
        }

        size_t sourceStart = getMinShownIndex(changes_[i]);
        size_t targetStart = sourceStart + targetIndexOffset;
        for (size_t k = i; k < j; ++k)
        {
            switch (changes_[k].change())
            {
                case simple_diff::Change::ADD:
                    ++targetIndexOffset;
                    break;
                case simple_diff::Change::REMOVE:
                    --targetIndexOffset;
                    break;
                default:
                    // no op
                    break;
            }
        }
        size_t sourceEnd = getMaxShownIndex(changes_[j - 1]);
        size_t targetEnd = sourceEnd + targetIndexOffset;

        result += "@@ " + std::to_string(sourceStart) + ":" + std::to_string(sourceEnd) + " "
                + std::to_string(targetStart) + ":" + std::to_string(targetEnd) + " @@\n";

        size_t lastShownIndex = sourceStart;
        result += "  " + getHexRepresentation(source_[sourceStart]) + " " + getHexRepresentation(source_[sourceStart]) + "\n";

        for (size_t k = i; k < j; ++k)
        {
            size_t needShowIndex = getMinShownIndex(changes_[k]);
            while (lastShownIndex < needShowIndex)
            {
                ++lastShownIndex;
                result += "  " + getHexRepresentation(source_[lastShownIndex]) + " "
                        + getHexRepresentation(source_[lastShownIndex]) + "\n";
            }
            lastShownIndex = needShowIndex;

            switch (changes_[k].change())
            {
                case simple_diff::Change::ADD:
                    result += "+ " + EMPTY_HEX + " " + getHexRepresentation(changes_[k].symbol()) + "\n";
                    break;
                case simple_diff::Change::REMOVE:
                    assert(lastShownIndex + 1 == changes_[k].index());
                    lastShownIndex = changes_[k].index();
                    result += "- " + getHexRepresentation(source_[lastShownIndex]) + " " + EMPTY_HEX + "\n";
                    break;
                case simple_diff::Change::REPLACE:
                    assert(lastShownIndex + 1 == changes_[k].index());
                    lastShownIndex = changes_[k].index();
                    result += "* " + getHexRepresentation(source_[lastShownIndex]) + " "
                              + getHexRepresentation(changes_[k].symbol()) + "\n";
                    break;
                default:
                    throw std::runtime_error("invalid change");
            }

        }

        assert(lastShownIndex == sourceEnd - 1);
        result += "  " + getHexRepresentation(source_[sourceEnd]) + " "
                  + getHexRepresentation(source_[sourceEnd]) + "\n";


        i = j;
    }
    return result;
}

simple_diff::Diff simple_diff::diffFromHumanReadableString(const std::vector<std::byte>& source, const std::string& string)
{
    std::vector<simple_diff::ChangeInstance> changes;

    std::istringstream ss(string);
    std::string line;
    size_t index;
    while (std::getline(ss, line))
    {
        switch (line[0])
        {
            case '@':
                index = readIndex(line);
                break;
            case '+':
                changes.emplace_back(index + 1, Change::ADD, getByte(line, false, false));
                break;
            case '-':
                changes.emplace_back(index++, Change::REMOVE, std::byte{0});
                break;
            case '*':
                changes.emplace_back(index++, Change::REPLACE, getByte(line, false, true));
                break;
            case ' ':
                ++index;
                break;
            default:
                throw std::runtime_error("invalid diff line: \"" + line + "\"");
        }
    }

    return simple_diff::Diff(source, changes);
}
