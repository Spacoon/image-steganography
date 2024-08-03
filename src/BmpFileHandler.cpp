#include "BmpFileHandler.h"

BmpFileHandler::BmpFileHandler(const std::string &path)
    : path(path)
{
    file = std::fstream(path, std::ios::in | std::ios::binary);
    if (!file.is_open())
    {
        fmt::print("You don't have permission to open {} file\n", path);
        return;
    }
    readHeader();
}

auto BmpFileHandler::readHeader() -> void
{
    headerInfo.resize(14);

    file.read(headerInfo.data(), sizeof(headerInfo));
    file.seekg(0, std::ios::beg);

    file.ignore(10);
    file.read(reinterpret_cast<char*>(&offsetDataPosition), sizeof(offsetDataPosition));

    file.read(reinterpret_cast<char*>(&size), sizeof(size));
    file.read(reinterpret_cast<char*>(&width), sizeof(width));
    file.read(reinterpret_cast<char*>(&height), sizeof(height));

    file.seekg(offsetDataPosition, std::ios::beg);
}

auto BmpFileHandler::encrypt(const std::string &message) -> void
{
    if (message.size() * 8 + 8 > width * height * 3)
    {
        fmt::println("This message is too long to be hidden in this image");
        return;
    }
    else
    {
        char c = 0;
        file.seekg(0, std::ios::beg);
        std::vector<char> info((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        // message encryption
        for (int i = 0; i < message.size() * 8; ++i)
        {
            info[offsetDataPosition - 1 + i] = (info[offsetDataPosition - 1 + i] & 0b11111110) | ((message[i / 8] >> (7 - i % 8)) & 1);
        }

        // hide a termination character
        for (int i = message.size() * 8; i < message.size() * 8 + 8; ++i)
        {
            info[offsetDataPosition - 1 + i] = (info[offsetDataPosition - 1 + i] & 0b11111110) | ((terminationCharacter >> (7 - i % 8)) & 1);
        }

        file.close();
        file.open(path, std::ios::out | std::ios::binary | std::ios::trunc);

        file.write(info.data(), info.size());
    }
}

auto BmpFileHandler::decrypt() -> void
{
    char c = 0;
    file.seekg(0, std::ios::beg);
    std::vector<char> info((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    bool isFinished = false;
    int indexOfMessage = 0;

    while (!isFinished)
    {
        c = 0;
        for (int i = 0; i < 8; ++i)
        {
            c |= (info[offsetDataPosition - 1 + i + indexOfMessage * 8] & 1) << (7 - i);
        }
        if (c == terminationCharacter)
        {
            isFinished = true;
            break;
        }
        indexOfMessage++;
        fmt::print("{}", c);
    }
}

auto BmpFileHandler::checkMessage(const std::string &message) -> void {
    using fmt::println;
    if (message.size() * 8 + 8 > width * height * 3)
    {
        println("This message is too long to be hidden in this image");
    }
    else
    {
        println("This message can be hidden in this image");
    }
}

auto BmpFileHandler::getDimensions() -> std::pair<int, int> {
    return {width, height};
}
