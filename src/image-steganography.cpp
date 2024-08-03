#include "App.h"

auto main(int argc, char* argv[]) -> int
{
    auto args = std::vector<std::string>(argv + 1, argv + argc);
    auto a = App(args);
}