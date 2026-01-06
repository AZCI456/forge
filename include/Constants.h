// Constants.hpp
#pragma once
#include <string_view> // read only access - no copying
#include <filesystem>

namespace fs = std::filesystem;

namespace config {
    // std::string_view = lighter and faster than std::string for constants
    constexpr std::string_view SOLUTION_NAME = "sol.cpp";
    constexpr std::string_view BRUTE_NAME = "bf.cpp";
    constexpr std::string_view GENERATOR_NAME= "gen.cpp";
    constexpr std::string_view TEST_EXT      = ".in";
    constexpr std::string_view VSOL_EXT      = ".out";

    constexpr std::string_view EDITOR      = "subl"; // shortcut I set up for sublime - use "code" for visual studio

    // basically infinite
    constexpr int GEN_SIZE = INT_MAX; // number of files to generate with gen.cpp

    // personal compile flag - just add to the end - maybe make list in future  - for tui
    constexpr std::string_view PERSONAL_FLAGS = " -fsanitize=address -g"; // not yet used

    // not constexpr as compiler can't use the heap to allocate the file text
    // inline = one definition per header file, const = runtime constant
    inline const fs::path GEN_FOLDER = "gen_tests";




}