#include "CLI.h"

#include <filesystem>
#include <fstream>
#include <iostream>

#include "Colours.h"
#include "Constants.h"
#include "ForgeFunct.h"
#include "Types.h"

#include "UtilityHeaders/ClipboardTools.h"
#include "UtilityHeaders/GeneratorTools.h"
#include "UtilityHeaders/InputTools.h"

namespace fs = std::filesystem;

// 1 for program success and -1 for failure
static int run_manual_build(const std::vector<std::string>& command_flags) {
    // either use the home directory or the provided one
    // we need to find the cleanest way to pass test cases separate from the solution files
    fs::path test_dir = !command_flags.empty() ? command_flags[0] : ".";

    const auto state = parse_directory(".", test_dir);
    if (state.valid_build()) {
        std::cout << "Success!" << "\n\n";
    } else {
        std::cout << colours::RED
                  << "CANNOT BUILD - ONE OF THE FOLLOWING FILES MISSING: SOL, TEST, ANS"
                  << colours::RESET << "\n";
        return -1;
    }

    std::vector<Task> tasks = create_plan(state);

    if (run_program(tasks)) {
        std::cout << "ALL TASKS SUCCESSFULLY COMPLETED\n\n";
    } else {
        std::cout << "AT LEAST ONE TASK FAILED... ABORTING!\n";
        return -1;
    }
    return 0;
}

int print_help() {
    std::cout << colours::CYAN << "Forge - Competitive Programming Tool" << colours::RESET << "\n";
    std::cout << "Usage:\n";
    std::cout << "  forge test         Run all local .in/.out test cases\n";
    std::cout << "  forge stress       Run the generator stress tester (gen/brute/sol)\n";
    std::cout << "  forge setup <dir>  Create a new project directory with sol.cpp\n";
    std::cout << "  forge in [tag]      Create sample input/output files quickly\n";
    std::cout << "                         -m <optional> explicit command mode\n";
    std::cout << "                         -pt <optional> print all existing tests\n";
    std::cout << "  forge help         Show this help message\n";
    std::cout << "  forge flags        Show detailed information about flags\n";

    return 0;
}

int print_flags() {
    std::cout << colours::CYAN << "Forge - Flags Information" << colours::RESET << "\n";
    std::cout << "Usage: forge <command> [flags]\n\n";

    std::cout << "Command: test\n";
    std::cout << "  forge test [test_dir]\n";
    std::cout << "  [test_dir]  Optional. Path to directory with test cases. "
                 "Defaults to current directory.\n\n";

    std::cout << "Command: stress\n";
    std::cout << "  forge stress [count]\n";
    std::cout << "  [count]     Optional. Number of tests to run. Defaults to "
                 "infinite.\n\n";

    std::cout << "Command: setup\n";
    std::cout << "  forge setup <dir>\n";
    std::cout << "  <dir>       Required. Name of the new project directory.\n\n";

    std::cout << "Command: in\n";
    std::cout << "  forge in [-m]\n";
    std::cout << "  [-m]         Optional. '-m' enables explicit command mode.\n";
    std::cout << "               In explicit mode, type '--' for output, '++' for "
                 "next case.\n";
    std::cout << "               Default (or '-m') uses Enter key for "
                 "navigation.\n";

    return 0;
}

static int run_stress(const std::vector<std::string>& command_flags) {
    int test_count = config::GEN_SIZE; // Default to config (infinite)

    if (!command_flags.empty()) {
        try {
            size_t pos;
            test_count = std::stoi(command_flags[0], &pos);
            // Ensure the entire string was parsed and positive
            if (pos != command_flags[0].size() || test_count <= 0) {
                throw std::invalid_argument("Invalid input");
            }
        } catch (const std::logic_error&) {
            std::cout << colours::RED << "Error: Invalid number of tests specified."
                      << colours::RESET << "\n";
            std::cout
                << "Please enter a valid integer value: [tests] > 0"
                << "\n";
            return -1;
        } catch (const std::exception& e) {
            std::cout << colours::RED
                      << "DEV: unaccounted for error - please contact developer "
                      << e.what() << colours::RESET << "\n";
            std::cout << "Press " << colours::CYAN << "'y'" << colours::RESET
                      << " to continue & anything else to abort: ";
            std::string input;
            std::cin >> input;
            if (input != "y") return -1;
        }
    }

    const auto state = parse_directory(".");
    if (state.valid_stress()) {
        std::cout << "Success!" << "\n\n";
    } else {
        // make it so the missing file is highlighted
        std::cout << colours::RED
                  << "CANNOT BUILD - ONE OF THE FOLLOWING FILES MISSING: SOL, "
                     "GEN, BF"
                  << colours::RESET << "\n";
        return -1;
    }

    // Run fuzzer: generates and tests incrementally, breaks on first failure
    return run_fuzzer(state, test_count);
}


void create_file(const std::string_view& name, const fs::path& project_dir) {
    fs::path file = project_dir / name;

    // create the files
    std::ofstream o_file(file);

    // check for file creation issues
    if (!o_file.is_open()) {
        std::cout << colours::RED << "Error: Could not create "
                << config::SOLUTION_NAME << colours::RESET << "\n";
        throw std::runtime_error("Could not create " + std::string(name));
    }
    std::cout << colours::GREEN << "Created file: " << name << colours::RESET << "\n";
}

static int setup_project(const std::vector<std::string>& command_flags) {
    // Check if a directory name was provided
    const std::string& dir_name = command_flags[0];

    if (dir_name.empty()) {
        std::cout << colours::RED << "Error: Please provide a directory name" << colours::RESET
                  << "\n";
        std::cout << "Usage: forge setup <directory_name>\n";
        return -1;
    }

    fs::path project_dir(dir_name);

    // Check if directory already exists
    if (fs::exists(project_dir)) {
        std::cout << colours::RED << "Error: Directory \"" << dir_name
                  << "\" already exists" << colours::RESET << "\n";
        return -1;
    }

    // Create directory
    try {
        fs::create_directory(project_dir);
        std::cout << colours::GREEN << "Created directory: " << dir_name << colours::RESET
                  << "\n";
    } catch (const fs::filesystem_error& e) {
        std::cout << colours::RED << "Error creating directory: " << e.what() << colours::RESET
                  << "\n";
        return -1;
    }

    // auto gen your needed files - LIST IN THE BRACKETS
    std::vector<std::string_view> initialise_files = {config::SOLUTION_NAME}; // Can add gen file etc if needed
    for (auto file : initialise_files) {
        create_file(file, project_dir);
    }

    // this is a requirement from above - still using the initialise_files as it's neater and more extensible
    // will refactor if I need to start with more files open
    fs::path sol_file = project_dir / config::SOLUTION_NAME;

    // use the editor to open the sol file
    std::string open_cmd =
        std::string(config::EDITOR) + " " + sol_file.string();
    std::cout << colours::CYAN << "Opening " << sol_file.string() << " in "
              << config::EDITOR << "..." << colours::RESET << "\n";
    if (std::system(open_cmd.c_str())) {
        std::cout << colours::RED << "Error: failure with the editor command "
                  << config::EDITOR << " " << sol_file.string() << colours::RESET
                  << "\n";
        std::cout << "Hint: Check Constants.h"
                  << "\n";
        return -1;
    }

    // copy directory to clipboard to quickly cd into
    std::string copy_paste_cmd = "cd " + std::string(dir_name);
    copyToClipboard(copy_paste_cmd);

    // misc info
    std::cout << colours::GREEN << "\nProject setup complete!" << colours::RESET << "\n";
    std::cout << "  Directory: " << fs::absolute(project_dir).string() << "\n";
    std::cout << "  Solution: " << sol_file.string() << "\n";
    std::cout << "\nPATH IS ON YOUR CLIPBOARD... To get started, run: cd "
              << dir_name << "\n";

    return 0;
}

/***
 * basic function that just runs a quick terminal script to create a REVIEW.MD in the current directory
 */
int run_review_command(const std::vector<std::string> & command_flags) {

    // set the command
    std::string system_command;
    if (! command_flags.empty() && command_flags[0] == "-v") { // handle subcommands
        system_command = "cat REVIEW.MD";
    }
    else system_command = "cat >> REVIEW.MD";

    // run the command
    std::cout << "--------------" << std::endl;
    if (std::system(system_command.c_str()) != 0) {
       std::cerr << "system operation for REVIEW.MD failed" << std::endl;
       return -1;
    }
    std::cout << "--------------" << std::endl;
    return 0;
}

int handle_command(const std::string& command,
                   const std::vector<std::string>& command_flags) {
    if (command == "help" || command == "--help" || command == "-h") {
        return print_help();
    }

    if (command == "flags") return print_flags();

    if (command == "test") return run_manual_build(command_flags);

    if (command == "stress") return run_stress(command_flags);

    if (command == "in") return handle_input_tests(command_flags);

    if (command == "review") return run_review_command(command_flags);

    if (command == "setup") {
        if (command_flags.empty()) {
            std::cout << colours::RED << "Error: Please provide a directory name"
                      << colours::RESET << "\n";
            std::cout << "Usage: forge setup <directory_name>\n";
            return -1;
        }
        return setup_project(command_flags);
    }

    std::cout << "Invalid command \"" << command << "\"\n\n";
    std::cout << "Try \"forge help\" \n\n";
    return -1;
}
