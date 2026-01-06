#include <iostream>
#include <filesystem>
#include <fstream>

#include "Types.h"
#include "ForgeFunct.h"
#include "Colours.h"
#include "Constants.h"
#include "../include/UtilityHeaders/GeneratorTools.h"
#include "../include/UtilityHeaders/InputTools.h"
#include "../include/UtilityHeaders/ClipboardTools.h"


namespace fs = std::filesystem;


// 1 for program success and -1 for failure
int run_manual_build(const std::vector<std::string> & command_flags) {

    // either use the home directory or the provided one
    // we need to find the cleanest way to pass test cases separate from the solution files
    fs::path test_dir = !command_flags.empty() ? command_flags[0] : ".";

    const auto state = parse_directory(".", test_dir);
    if (state.valid_build()) {
        std::cout << "Success!" << "\n\n";
    }
    else {
        std::cout << RED << "CANNOT BUILD - ONE OF THE FOLLOWING FILES MISSING: SOL, TEST, ANS" << RESET << "\n";
        return -1;
    }

    std::vector<Task> tasks = create_plan(state);

    //
    // if (command_flags.size() > 1 && run_program(tasks)) std::cout << "ALL TASKS SUCCESSFULLY COMPLETED\n\n";
    if (run_program(tasks)) std::cout << "ALL TASKS SUCCESSFULLY COMPLETED\n\n";
    else {
        std::cout << "AT LEAST ONE TASK FAILED... ABORTING!\n";
        return -1;
    }
    return 0;
}

// should we implement namespaces for the colours COLOUR::CYAN
int print_help() {

    std::cout << CYAN << "Forge - Competitive Programming Tool" << RESET << "\n";
    std::cout << "Usage:\n";
    std::cout << "  forge test       Run all local .in/.out test cases\n";
    std::cout << "  forge stress     Run the generator stress tester (gen/brute/sol)\n";
    std::cout << "  forge setup <dir> Create a new project directory with sol.cpp\n";
    std::cout << "  forge in <p>  Create sample input/output files quickly\n";
    std::cout << "                    p <optional> if you want enter to move to next section\n";

    std::cout << "  forge help       Show this help message\n";
    std::cout << "  forge flags      Show detailed information about flags\n";

    return 0;
};

int print_flags() {
    std::cout << CYAN << "Forge - Flags Information" << RESET << "\n";
    std::cout << "Usage: forge <command> [flags]\n\n";

    std::cout << "Command: test\n";
    std::cout << "  forge test [test_dir]\n";
    std::cout << "  [test_dir]  Optional. Path to directory with test cases. Defaults to current directory.\n\n";

    std::cout << "Command: stress\n";
    std::cout << "  forge stress [count]\n";
    std::cout << "  [count]     Optional. Number of tests to run. Defaults to infinite.\n\n";

    std::cout << "Command: setup\n";
    std::cout << "  forge setup <dir>\n";
    std::cout << "  <dir>       Required. Name of the new project directory.\n\n";

    std::cout << "Command: in\n";
    std::cout << "  forge in [-m]\n";
    std::cout << "  [-m]         Optional. '-m' enables explicit command mode.\n";
    std::cout << "              In explicit mode, type '--' for output, '++' for next case.\n";
    std::cout << "              Default (or '-m') uses Enter key for navigation.\n";

    return 0;
}

// maybe put in the state struct or promote to class


int run_stress(const std::vector<std::string> & command_flags) {
    int test_count = config::GEN_SIZE; // Default to config (infinite)
    
    if (!command_flags.empty()) {
        try {
            test_count = std::stoi(command_flags[0]);
        }
        catch (const std::invalid_argument& e_invalid_arg) {
            std::cout << RED << "Error: Invalid number of tests specified." << RESET << "\n";
            return -1;
        }
        catch (const std::exception& e) {
            std::cout << RED << "DEV: unaccounted for error - please contact developer" << e.what() <<RESET << "\n";
            std::cout << "Press " << CYAN << "\'y\'" << RESET << " to continue & anything else to abort: "<< ""; //  RED << "N" << RESET << "to abort"<<
            std::string input; std::cin >> input;
            if (input != "y") return -1;
        }
    }

    // this segment can be abstracted into the create plan function to avoid repetition (just need to see how to differentiate the output text and the function call valid stress) perhaps use inheritance instead of a struct for stress test verse manual test
    const auto state = parse_directory(".");
    if (state.valid_stress()) {
        std::cout << "Success!" << "\n\n";
    }
    else {
        // make it so the missing file is highlighted
        std::cout << RED << "CANNOT BUILD - ONE OF THE FOLLOWING FILES MISSING: SOL, GEN, BF" << RESET << "\n";
        return -1;
    }
    
    // Run fuzzer: generates and tests incrementally, breaks on first failure
    // Note: You must update the definition of run_fuzzer to accept this int
    return run_fuzzer(state, test_count);
};


int setup_project(const std::vector<std::string> & command_flags) { // not suer on the overhead difference to using const char*
    // Check if a directory name was provided
    const std::string & dir_name = command_flags[0];

    if (dir_name.empty()) {
        std::cout << RED << "Error: Please provide a directory name" << RESET << "\n";
        std::cout << "Usage: forge setup <directory_name>\n";
        return -1;
    }
    
    fs::path project_dir(dir_name);
    
    // Check if directory already exists
    if (fs::exists(project_dir)) {
        std::cout << RED << "Error: Directory \"" << dir_name << "\" already exists" << RESET << "\n";
        return -1;
    }
    
    // Create directory
    try {
        fs::create_directory(project_dir);
        std::cout << GREEN << "Created directory: " << dir_name << RESET << "\n";
    } catch (const fs::filesystem_error& e) {
        std::cout << RED << "Error creating directory: " << e.what() << RESET << "\n";
        return -1;
    }
    
    // Create sol.cpp file and gen file
    fs::path sol_file = project_dir / config::SOLUTION_NAME;
    fs::path gen_file = project_dir / config::GENERATOR_NAME;

    // create the file  - check the ofstream initalisaiton was successful
    std::ofstream s_file(sol_file);
    std::ofstream g_file(gen_file);
    if (!s_file.is_open()) {
        std::cout << RED << "Error: Could not create " << config::SOLUTION_NAME << RESET << "\n";
        return -1;
    }
    std::cout << GREEN << "Created file: " << sol_file.string() << RESET << "\n";

    if (! g_file.is_open()) {
        std::cout << RED << "Error: Could not create " << config::GENERATOR_NAME << RESET << "\n";
        return -1;
    }
    std::cout << GREEN << "Created file: " << gen_file.string() << RESET << "\n";


    // use the editor to open the sol file
    std::string open_cmd = std::string(config::EDITOR) + " " + sol_file.string();
    std::cout << CYAN << "Opening " << sol_file.string() << " in " << config::EDITOR << "..." << RESET << "\n";
    if(std::system(open_cmd.c_str())) {
        std::cout << RED << "Error: failure with the editor command " << config::EDITOR << " " << sol_file.string() << RESET << "\n";
        std::cout <<  "Hint: Check Constants.h" << "\n";
        return -1;
    }

    // copy directory to clipboard to quickly cd into
    std::string copy_paste_cmd = "cd " + std::string(dir_name);
    copyToClipboard(copy_paste_cmd); // using a lot of online boilerplate magic

    // misc info
    std::cout << GREEN << "\nProject setup complete!" << RESET << "\n";
    std::cout << "  Directory: " << fs::absolute(project_dir).string() << "\n";
    std::cout << "  Solution: " << sol_file.string() << "\n";
    std::cout << "\nPATH IS ON YOUR CLIPBOARD... To get started, run: cd " << dir_name << "\n";
    
    return 0;
}


int main(int argc, char** argv) {

    if (argc < 2) {
        print_help();
        return 0;
    }

    // easier handling
    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i) {
        args.emplace_back(argv[i]);
    }

    std::string command = args[0];
    // Create a new vector containing only the flags (arguments after the command)
    std::vector<std::string> command_flags;
    if (args.size() > 1) {
        command_flags.assign(args.begin() + 1, args.end());
    }

    if (command == "help" || command == "--help" || command == "-h") print_help();

    else if (command == "flags") print_flags();

    else if (command == "test") return run_manual_build(command_flags);

    else if (command == "stress") return run_stress(command_flags);

    // flag -m for manual mode, (explicit characters for newlines
    else if (command == "in")  return handle_input_tests(command_flags);

   // else if (command == "copy") mv current sol file and make new sol file
    else if (command == "setup") {
        if (argc < 3) {
            std::cout << RED << "Error: Please provide a directory name" << RESET << "\n";
            std::cout << "Usage: forge setup <directory_name>\n";
            return -1;
        }
        return setup_project(command_flags);
    }

    else {
        std::cout << "Invalid command \"" << command << "\"\n\n";
        std::cout << "Try \"forge help\" \n\n";
    }


    return 0;
}
