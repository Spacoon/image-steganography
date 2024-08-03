#pragma once

#include <string>
#include <ctime>
#include <fstream>
#include <filesystem>
#include <fmt/core.h>
#include <vector>

class PpmFileHandler {
public:
    PpmFileHandler(std::string const& path);
    auto decrypt() -> void;
    auto encrypt(std::string const& message) -> void;
    auto checkMessage(std::string const& message) -> void;
    auto getDimensions() -> std::pair<int, int>;
private:
    std::string path;
    std::fstream file;

    std::string magicNumber;
    int width = 0, height = 0, maxColorValue = 0;
    std::vector<std::string> fileHeaderInfo;

    auto readHeader() -> void;

    char terminationCharacter = 127; // DEL character
};

