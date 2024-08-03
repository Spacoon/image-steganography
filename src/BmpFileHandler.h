#pragma once

#include <fmt/core.h>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>

class BmpFileHandler
{
public:
    BmpFileHandler(std::string const& path);
    auto decrypt() -> void;
    auto encrypt(std::string const& message) -> void;
    auto checkMessage(std::string const& message) -> void;
    auto getDimensions() -> std::pair<int, int>;
private:
    std::string path;
    std::fstream file;

    auto readHeader() -> void;
    int offsetDataPosition;
    int size = 0, width = 0, height = 0;

    std::vector<char> headerInfo;

    char terminationCharacter = 127; // DEL character
};