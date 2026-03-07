# Forge - Competitive Programming Tool
# Requirements: CMake 3.20+, C++17 compiler (e.g. GCC 8+, Clang 6+, MSVC 2017+).
# No external libraries; uses C++ standard library and <filesystem>.

cmake_minimum_required(VERSION 3.20)
project(forge)

set(CMAKE_CXX_STANDARD 17)

# Build output: executable in build/ (or build/Debug|Release with multi-config generators)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(include include/UtilityHeaders)

add_executable(forge
        src/main.cpp
        src/cli.cpp
        src/directory_parser.cpp
        src/planner.cpp
        src/executor.cpp
        src/utils/GeneratorTools.cpp
        src/utils/FilesystemInputTools.cpp
        include/UtilityHeaders/FilesystemInputTools.h
        src/utils/ClipboardTools.cpp
        include/UtilityHeaders/ClipboardTools.h
        src/error_messages.cpp
        src/error_messages.h
        src/utils/UserInputTools.cpp
)