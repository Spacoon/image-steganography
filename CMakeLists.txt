cmake_minimum_required(VERSION 3.0)
project(image_steganography)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-static")

# set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

include(FetchContent)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY  https://github.com/fmtlib/fmt
        GIT_TAG         10.2.1
)

FetchContent_MakeAvailable(fmt)

add_executable(image_steganography
        src/image-steganography.cpp
        src/App.h
        src/App.cpp
        src/PpmFileHandler.cpp
        src/PpmFileHandler.h
        src/BmpFileHandler.cpp
        src/BmpFileHandler.h)
target_link_libraries(image_steganography fmt)