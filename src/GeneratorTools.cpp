

#include "GeneratorTools.h"

#include "Constants.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

// see folder exists if not create folder
void prepareDirectory(const fs::path & folder) {
    // Create directory if it doesn't exist; do nothing if it does.
    if (!std::filesystem::exists(folder)) {
        std::filesystem::create_directory(folder);
    }
}

void compile_file(const std::string_view & file, const std::string& name) {
    std::cout << "  Compiling Generator... ";
    if (std::system(("clang++ -std=c++17 " + std::string(file) + " -o" + name).c_str()) != 0) {
        std::cerr << "FAILED. Check " << name << " for errors.\n";
        return;
    }
}

// Generate tests
// should create a compiler warning when you're running gen tests for the second time
// currently only works for Mac needs to pause in the path to become universal
void gen_tests() {

    prepareDirectory(config::GEN_FOLDER);

    compile_file(config::GENERATOR_NAME, "gen");
    compile_file(config::BRUTE_NAME, "bf");

    std::system("");
    for (int i = 0; i < 3; i++) {
        // generate a file and put it in the generator folder
        // Will this work on all build systems?
        std::string command = "./gen > ./" + config::GEN_FOLDER.string() + "/" + std::to_string(i) + ".in";
        std::system(command.c_str());
        command = "./bf < ./" + config::GEN_FOLDER.string() + "/" + std::to_string(i) + ".in >  ./" + config::GEN_FOLDER.string() + std::to_string(i) + ".out";
        std::system(command.c_str());
    }
}