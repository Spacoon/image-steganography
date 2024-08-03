#include "PpmFileHandler.h"

PpmFileHandler::PpmFileHandler(std::string const& path)
    : path(path)
{
    file = std::fstream(path, std::ios::in);
    if (!file.is_open())
    {
        fmt::print("You don't have permission to open {} file\n", path);
        return;
    }
    readHeader();
}

auto PpmFileHandler::readHeader() -> void // https://netpbm.sourceforge.net/doc/ppm.html
{
    using fmt::println, fmt::print;

    std::string text;

    file >> magicNumber;

    if (magicNumber.empty())
    {
        println("The file is empty.");
        return;
    }

    fileHeaderInfo.push_back(magicNumber);

    for (int i = 0; i < 3; ++i)
    {
        file >> text;

        if (text[0] == '#')
        {
            std::getline(file, text);
            text = "#" + text;
            i--;
            fileHeaderInfo.push_back(text);
        }
        else
        {
            switch (i)
            {
                case 0:
                    width = std::stoi(text);
                    fileHeaderInfo.push_back(text);
                    break;
                case 1:
                    height = std::stoi(text);
                    fileHeaderInfo.push_back(text);
                    break;
                case 2:
                    maxColorValue = std::stoi(text);
                    fileHeaderInfo.push_back(text);
                    break;
                default:
                    println("Something went wrong");
                    break;
            }
        }
    }
    file.ignore();
}

auto PpmFileHandler::decrypt() -> void
{
    using fmt::println, fmt::print;

    if (magicNumber == "P3")
    {
        std::vector<int> rgbPixelValues;
        std::string colorRead;

        int counter = 0;
        while (file >> colorRead)
            counter++;

        colorRead = "";
        char c = 0;

        file.close();

        file.open(path, std::ios::in);

        readHeader();

        for (int i = 0; i < counter; ++i)
        {
            file >> colorRead;
            c |= (std::stoi(colorRead) & 1) << ((7 - i % 8));
            if (c == terminationCharacter)
                return;

            if ((i + 1) % 8 == 0 and i != 0)
            {
                print("{}", c);
                c = 0;
            }
        }
    }
    else if (magicNumber == "P6")
    {
        file.close();
        file.open(path, std::ios::in | std::ios::out | std::ios::binary);
        fileHeaderInfo.erase(fileHeaderInfo.begin(), fileHeaderInfo.end());
        readHeader();

        std::vector<char> data(width * height * 3);

        file.read(reinterpret_cast<char*>(data.data()), data.size());

        char c = 0;
        for (int i = 0; i < width * height * 3; ++i)
        {
            c |= (data[i] & 1) << ((7 - i % 8));

            if (c == terminationCharacter)
                return;
            if ((i + 1) % 8 == 0 and i != 0)
            {
                print("{}", c);
                c = 0;
            }
        }
    }
}

auto PpmFileHandler::encrypt(const std::string &message) -> void
{
    using fmt::println, fmt::print;

    std::vector<int> rgbPixelValues;
    if (magicNumber == "P3")
    {
        std::string colorRead;
        while (file >> colorRead)
        {
            rgbPixelValues.push_back(std::stoi(colorRead));
        }

        if (message.size() * 8 + 8 > rgbPixelValues.size())
        {
            println("This message is too long to be hidden in this image");
            return;
        }

        // message encryption
        for (int i = 0; i < message.size() * 8; ++i)
        {
            rgbPixelValues[i] = (rgbPixelValues[i] & 0b11111110) | ((message[i / 8] >> (7 - i % 8)) & 1);
        }

        // hide a termination character
        for (int i = message.size() * 8; i < message.size() * 8 + 8; ++i)
        {
            rgbPixelValues[i] = (rgbPixelValues[i] & 0b11111110) | ((terminationCharacter >> (7 - i % 8)) & 1);
        }

        file.close();
        file.open(path, std::ios::in | std::ios::out | std::ios::trunc);

        int infoCounter = 0;

        for (int i = 0; i < fileHeaderInfo.size(); ++i)
        {
            if (i == 0) // magic number
            {
                file << fileHeaderInfo[i] << "\n";
                infoCounter++;
            }
            else if (fileHeaderInfo[i][0] == '#') // comments
            {
                file << fileHeaderInfo[i] << "\n";
            }
            else if (infoCounter == 1) // width and height
            {
                file << fileHeaderInfo[i] << " " << fileHeaderInfo[i + 1] << "\n";
                i++;
                infoCounter += 2;
            }
            else if (infoCounter == 3) // max color value
            {
                file << fileHeaderInfo[i] << "\n";
                infoCounter++;
            }
            else
            {
                file << fileHeaderInfo[i];
            }
        }

        for (int i = 0; i < rgbPixelValues.size(); ++i)
        {
            if (i % (width * 3) == 0 and i != 0)
            {
                file << "\n";
            }
            file << rgbPixelValues[i] << " ";
        }
    }
    else if (magicNumber == "P6")
    {
        file.close();
        file.open(path, std::ios::in | std::ios::out | std::ios::binary);
        fileHeaderInfo.erase(fileHeaderInfo.begin(), fileHeaderInfo.end());
        readHeader();

        if (message.size() * 8 + 8 > width * height * 3)
        {
            println("This message is too long to be hidden in this image");
            return;
        }

        std::vector<char> data(width * height * 3);
        file.read(data.data(), data.size());

        for (int i = 0; i < message.size() * 8; ++i)
        {
            data[i] = (data[i] & 0b11111110) | ((message[i / 8] >> (7 - i % 8)) & 1);
        }

        // hide a termination character
        for (int i = message.size() * 8; i < message.size() * 8 + 8; ++i)
        {
            data[i] = (data[i] & 0b11111110) | ((terminationCharacter >> (7 - i % 8)) & 1);
        }

         file.close();
         file.open(path, std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);

         int infoCounter = 0;

         for (int i = 0; i < fileHeaderInfo.size(); ++i)
         {
             if (i == 0) // magic number
             {
                 file << fileHeaderInfo[i] << "\n";
                 infoCounter++;
             }
             else if (fileHeaderInfo[i][0] == '#')
             {
                 file << fileHeaderInfo[i] << "\n";
             }
             else if (infoCounter == 1) // width and height
             {
                 file << fileHeaderInfo[i] << " " << fileHeaderInfo[i + 1] << "\n";
                 i++;
                 infoCounter += 2;
             }
             else if (infoCounter == 3) // max color value
             {
                 file << fileHeaderInfo[i] << "\n";
                 infoCounter++;
             }
             else
             {
                 file << fileHeaderInfo[i];
             }
         }
         file.write(reinterpret_cast<char*>(data.data()), data.size());
    }
}

auto PpmFileHandler::checkMessage(const std::string &message) -> void
{
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

auto PpmFileHandler::getDimensions() -> std::pair<int, int> {
    return {width, height};
}
