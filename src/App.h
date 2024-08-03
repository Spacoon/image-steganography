#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <filesystem>
#include <chrono>
#include <chrono>
#include <format>

#include "PpmFileHandler.h"
#include "BmpFileHandler.h"

class App
{
public:
	App(std::vector<std::string> const& args);

private:
	std::vector<std::string> args = {};

    std::fstream file;
    std::string magicNumber;

	auto handleFlags() -> void;
    static auto viewProgramHelp() -> void;
    auto viewFileInfo(std::pair<int, int> const& dimensions, std::string const& fileExtension) -> void;
};
