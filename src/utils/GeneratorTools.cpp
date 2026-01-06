

#include "../../include/UtilityHeaders/GeneratorTools.h"

#include "Constants.h"
#include "Colours.h"
#include <filesystem>
#include <iostream>
#include <string>

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
    if (std::system(("clang++ -std=c++17 " +  std::string(file) + " -o" + name).c_str()) != 0) {
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
        command = "./bf < ./" + config::GEN_FOLDER.string() + "/" + std::to_string(i) + ".in >  ./" + config::GEN_FOLDER.string() + "/" + std::to_string(i) + ".out";
        std::system(command.c_str());
    }
}

// extracted from the original function to increase readability.
//
void prompt_user_for_input(int test_num, const std::string& in_file) {
    std::cout << CYAN << "\nWould you like to see the input file? (Y/N): " << RESET;
    std::string response;
    std::cin >> response;

    if (response == "Y" || response == "y") {
        std::cout << CYAN << "\n--- Input file for test case " << test_num << " ---" << RESET << "\n";
        std::string cat_cmd = "cat " + in_file;
        std::system(cat_cmd.c_str());
        std::cout << CYAN << "--- End of input file ---" << RESET << "\n\n";
    }
}

// Generate and test one fuzzer test case, returns true if test passed
bool generate_and_test_one(int test_num) {
    namespace fs = std::filesystem;
    
    // Generate input file
    std::string in_file = config::GEN_FOLDER.string() + "/" + std::to_string(test_num) + ".in";
    std::string command = "./gen > " + in_file;
    if (std::system(command.c_str()) != 0) {
        std::cerr << RED << "Failed to generate test case " << test_num << RESET << "\n";
        return false;
    }
    
    // Generate expected output using brute force
    std::string expected_out = config::GEN_FOLDER.string() + "/" + std::to_string(test_num) + ".out";
    command = "./bf < " + in_file + " > " + expected_out;
    if (std::system(command.c_str()) != 0) {
        std::cerr << RED << "Failed to generate expected output for test case " << test_num << RESET << "\n";
        return false;
    }
    
    // Run solution and generate output
    std::string sol_out = config::GEN_FOLDER.string() + "/s_" + std::to_string(test_num) + ".out";
    command = "./sol < " + in_file + " > " + sol_out;
    if (std::system(command.c_str()) != 0) {
        std::cout << RED << "[FAILED] - Runtime error on test case " << test_num << RESET << "\n";
        return false;
    }
    
    // 2. CHECK for correctness (Silent)
    // We use 'diff -q' (quiet) which returns 1 if files differ, 0 if same
    // /dev/null = 'bit bucket' => trash the stdout and stderr (2>&1 => 2nd output stream - error - to stdout - which goes to bin)
    std::string check_cmd = "diff -q -w " + sol_out + " " + expected_out + " > /dev/null 2>&1";

    if (std::system(check_cmd.c_str()) != 0) {
        // 3. ONLY print the visual diff if it FAILED
        std::cout << RED << "\n[FAILED] - Output mismatch on test case " << test_num << RESET << "\n";
        
        // NOW we run the expensive visual diff to show the user what happened
        std::string view_cmd = "colordiff -y " + sol_out + " " + expected_out + " || diff -y " + sol_out + " " + expected_out;
        std::system((view_cmd).c_str());
        
        // Ask user if they want to see the input file
        prompt_user_for_input(test_num, in_file);

        return false; // Stop the loop
    }

    // Optional: can switch to dot mode if more satisfying - just replaced text with "."
    //  not plus one because now in the past
    std::cout << "\r" << "Tests Passed: [" << test_num << "]" << std::flush;
    return true;
}

// Run fuzzer: generate and test incrementally, breaking on first failure
int run_fuzzer(const FolderState& state, const int test_count) { // test count passed in as const
    prepareDirectory(config::GEN_FOLDER);
    
    // Compile generator and brute force
    compile_file(config::GENERATOR_NAME, "gen");
    compile_file(config::BRUTE_NAME, "bf");
    
    // Compile solution
    std::string compile_cmd = "clang++ -std=c++17 " + state.sol_path.string() + " -o sol";
    std::cout << "COMPILING [SOLUTION]..." << '\n';
    if (std::system(compile_cmd.c_str()) != 0) {
        std::cerr << RED << "Failed to compile solution" << RESET << "\n";
        return -1;
    }
    std::cout << GREEN << "[SUCCESSFUL]" << RESET << "\n\n";

    //
    // Generate and test incrementally
    std::cout << "RUNNING FUZZER TESTS..." << '\n';
    std::cout << "If on infinite (default mode press CTRL+C) to quit  " << '\n';
    std::cout << "(quit case will show as failed but that is most likely a false positive " << '\n';

    for (int i = 0; test_count == -1 || i < test_count; i++) {
        if (!generate_and_test_one(i)) {
            std::cout << RED << "\nFUZZER STOPPED: Test case " << i << " failed" << RESET << "\n";
            return -1;
        }
    }
    
    std::cout << GREEN << "\nALL FUZZER TESTS PASSED (" << test_count << " test cases)" << RESET << "\n";
    return 0;
}