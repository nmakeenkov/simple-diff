#include "simple_diff.h"
#include <iostream>
#include <cstddef>
#include <iomanip>
#include <fstream>

namespace
{
std::vector<std::byte> readFile(char* path)
{
    std::ifstream infile(path);

    infile.seekg(0, std::ios::end);
    size_t length = static_cast<size_t>(infile.tellg());
    infile.seekg(0, std::ios::beg);

    std::vector<std::byte> buffer(length);

    infile.read(reinterpret_cast<char*>(buffer.data()), length);

    return buffer;
}
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Bad arguments\n";
        return 1;
    }

    auto source = readFile(argv[1]);
    auto target = readFile(argv[2]);

    auto diff = simple_diff::computeDiff(source, target);

    std::cout << diff.toHumanReadable() << "\n";
    return 0;
}
