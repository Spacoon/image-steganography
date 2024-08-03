#include "App.h"

App::App(std::vector<std::string> const& args)
	: args(args)
{
    handleFlags();
}

auto App::handleFlags() -> void
{
	using fmt::println;
	namespace fs = std::filesystem;

    if (args.size() > 1)
    {
        if (fs::exists(args[1]))
        {
            fs::path path = args[1];
            std::string fileExtension = path.extension().string();

            if (args[0] == "-i" or args[0] == "--info") {
                if (args.size() < 3) {
                    if (fileExtension == ".ppm") {
                        auto fileHandler = PpmFileHandler(args[1]);
                        viewFileInfo(fileHandler.getDimensions(), fileExtension);
                    } else if (fileExtension == ".bmp") {
                        auto fileHandler = BmpFileHandler(args[1]);
                        viewFileInfo(fileHandler.getDimensions(), fileExtension);
                    } else {
                        println("This file is not supported");
                        return;
                    }
                } else {
                    println("Too many arguments, use -h or --help to view available flags");
                }
            } else if (args[0] == "-e" or args[0] == "--encrypt") {
                if (args.size() < 4) {
                    if (fileExtension == ".ppm") {
                        auto fileHandler = PpmFileHandler(args[1]);
                        fileHandler.encrypt(args[2]);
                    } else if (fileExtension == ".bmp") {
                        auto fileHandler = BmpFileHandler(args[1]);
                        fileHandler.encrypt(args[2]);
                    } else {
                        println("This file is not supported");
                        return;
                    }
                } else {
                    println("Too many arguments, use -h or --help to view available flags");
                    return;
                }
            } else if (args[0] == "-d" or args[0] == "--decrypt") {
                if (args.size() < 3) {
                    if (fileExtension == ".ppm") {
                        auto fileHandler = PpmFileHandler(args[1]);
                        fileHandler.decrypt();
                    } else if (fileExtension == ".bmp") {
                        auto fileHandler = BmpFileHandler(args[1]);
                        fileHandler.decrypt();
                    } else {
                        println("This file is not supported");
                        return;
                    }
                } else {
                    println("Too many arguments, use -h or --help to view available flags");
                    return;
                }
            } else if (args[0] == "-c" or args[0] == "--check") {
                if (args.size() == 3) {
                    if (fileExtension == ".ppm") {
                        auto fileHandler = PpmFileHandler(args[1]);
                        fileHandler.checkMessage(args[2]);
                    } else if (fileExtension == ".bmp") {
                        auto fileHandler = BmpFileHandler(args[1]);
                        fileHandler.checkMessage(args[2]);
                    } else {
                        println("This file is not supported");
                        return;
                    }
                } else {
                    println("Too many arguments, use -h or --help to view available flags");
                    return;
                }
            } else if (args[0] == "-h" or args[0] == "--help") {
                if (args.size() < 2) {
                    viewProgramHelp();
                } else {
                    println("Too many arguments, use -h or --help to view available flags");
                    return;
                }
            } else {
                println("Unknown flag, use -h or --help to view available flags");
            }
        }
        else
        {
            println("This file does not exist");
        }
    }
    else
    {
        viewProgramHelp();
    }
}

auto App::viewProgramHelp() -> void
{
    fmt::println("Application that hides messages in image using steganography\n"
            "Options:\n"
            "-i, --info\tView information about the image\n"
            "-e, --encrypt\tencrypt a message in a supported image format\n"
            "-d, --decrypt\tdecrypt a message from a supported image format\n"
            "-c, --check\tcheck if your message can be encrypted in a supported image format\n"
            "-h, --help\tview help information\n"
    );
}

auto App::viewFileInfo(std::pair<int, int> const& dimensions, std::string const& fileExtension) -> void
{
    using fmt::println;
    namespace fs = std::filesystem;

    auto lastModifiedTime = std::filesystem::last_write_time(args[1]); // https://en.cppreference.com/w/cpp/filesystem/last_write_time
    auto fileSize = fs::file_size(args[1]);

    std::cout << fileExtension << " file"
                 "\nWidth: " << dimensions.first <<
                 "\nHeight: " << dimensions.second <<
                 "\nSize: " << fileSize << " bytes"
                 "\nLast modification: " << std::format("Time: {} ", lastModifiedTime) << "\n";
}