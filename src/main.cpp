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
int run_manual_build() {
    const auto state = parse_directory(".");
    if (state.valid_build()) {
        std::cout << "Success!" << "\n\n";
    }
    else {
        std::cout << RED << "CANNOT BUILD - ONE OF THE FOLLOWING FILES MISSING: SOL, TEST, ANS" << RESET << "\n";
        return -1;
    }

    std::vector<Task> tasks = create_plan(state);

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

    return 0;
};

// maybe put in the state struct or promote to class


int run_stress() {
    // this segment can be abstracted into the create plan function to avoid repetition
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
    return run_fuzzer(state);
};


int setup_project(const char* dir_name) {
    // Check if directory name was provided
    if (dir_name == nullptr || std::string(dir_name).empty()) {
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
    
    // Create sol.cpp file
    fs::path sol_file = project_dir / config::SOLUTION_NAME;

    // create the file  - check the ofstream initalisaiton was successful
    std::ofstream file(sol_file);
    if (!file.is_open()) {
        std::cout << RED << "Error: Could not create " << config::SOLUTION_NAME << RESET << "\n";
        return -1;
    }
    std::cout << GREEN << "Created file: " << sol_file.string() << RESET << "\n";


    // use the editor to open the file
    std::string open_cmd = std::string(config::EDITOR) + " " + sol_file.string();
    std::cout << CYAN << "Opening " << sol_file.string() << " in " << config::EDITOR << "..." << RESET << "\n";
    if(std::system(open_cmd.c_str())) {
        std::cout << RED << "Error: failure with the editor command " << config::EDITOR << " " << sol_file.string() << RESET << "\n";
        std::cout <<  "Hint: Check Constants.h" << "\n";
        return -1;
    }

    std::string copy_paste_cmd = "cd " + std::string(dir_name);
    copyToClipboard(copy_paste_cmd); // using a lot of online boilerplate magic

    // misc info
    std::cout << GREEN << "\nProject setup complete!" << RESET << "\n";
    std::cout << "  Directory: " << fs::absolute(project_dir).string() << "\n";
    std::cout << "  Solution: " << sol_file.string() << "\n";
    std::cout << "\nTo get started, run: cd " << dir_name << "\n";
    
    return 0;
}


int main(int argc, char** argv) {

    if (argc < 2) {
        print_help();
        return 0;
    }

    std::string command = argv[1];
    if (command == "help" || command == "--help" || command == "-h") print_help();

    else if (command == "test") return run_manual_build();

    else if (command == "stress") return run_stress();

    else if (command == "in") {
        if (argc >= 3) {
            return handle_input_tests(true); // Third argument present, use copy_paste mode
        }
        return handle_input_tests(false); // No third argument, use Enter key mode
    }

    else if (command == "setup") {
        if (argc < 3) {
            std::cout << RED << "Error: Please provide a directory name" << RESET << "\n";
            std::cout << "Usage: forge setup <directory_name>\n";
            return -1;
        }
        return setup_project(argv[2]);
    }

    else {
        std::cout << "Invalid command \"" << command << "\"\n\n";
        std::cout << "Try \"forge help\" \n\n";
    }


    return 0;
}
