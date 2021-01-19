#include "simple_diff.h"
#include <iostream>
#include <fstream>

namespace
{
std::vector<std::byte> readFile(char* path)
{
    std::ifstream infile(path, std::ios_base::binary);

    infile.seekg(0, std::ios::end);
    size_t length = static_cast<size_t>(infile.tellg());
    infile.seekg(0, std::ios::beg);

    std::vector<std::byte> buffer(length);

    infile.read(reinterpret_cast<char*>(buffer.data()), length);

    return buffer;
}

std::string readString(char* path)
{
    std::ifstream infile(path);

    return std::string((std::istreambuf_iterator<char>(infile)),
            std::istreambuf_iterator<char>());
}

void writeToFile(char* path, const std::vector<std::byte>& data)
{
    std::ofstream outfile(path, std::ios_base::binary);

    outfile.write(reinterpret_cast<const char*>(data.data()), data.size());
}
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cerr << "Bad arguments\n";
        return 1;
    }

    auto source = readFile(argv[1]);
    auto string = readString(argv[2]);

    auto diff = simple_diff::diffFromHumanReadableString(source, string);

    auto result = diff.computeResult();

    writeToFile(argv[3], result);

    return 0;
}
