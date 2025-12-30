#include <iostream>
#include <filesystem>

#include "Types.h"
#include "ForgeFunct.h"
#include "Colours.h"
#include "Constants.h"
#include "GeneratorTools.h"


// 1 for program success and -1 for failure
int run_manual_build() {
    auto state = parse_directory(".");
    if (state.valid_build()) {
        std::cout << "Success!" << "\n\n";
    }
    else {
        std::cout << RED << "CANNOT BUILD - ONE OF THE ABOVE FILES MISSING" << RESET << "\n";
        return -1;
    }

    std::vector<Task> tasks = create_plan(state);

    // // testing functionality
    // for (auto task: tasks){
    //     std::cout << task.name << '\n';
    //     std::cout << task.command << '\n';
    //     std::cout << "DEPENDENCIES: " << '\n';
    //     for (auto dependency: task.dependencies){
    //         std::cout << '\t' << dependency << '\n';
    //     }
    //     std::cout << "\n ------------- \n";
    // }

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
    std::cout << "  forge help       Show this help message\n";

    return 0;
};

// maybe put in the state struct or promote to class


int run_stress() {
    // this segment can be abstracted into the create plan function to avoid repetition
    auto state = parse_directory(".");
    if (state.valid_stress()) {
        std::cout << "Success!" << "\n\n";
    }
    else {
        // make it so the missing file is highlighted
        std::cout << RED << "CANNOT BUILD - ONE OF THE ABOVE FILES MISSING" << RESET << "\n";
        return -1;
    }
    
    // Run fuzzer: generates and tests incrementally, breaks on first failure
    return run_fuzzer(state);
};

int main(int argc, char** argv) {

    if (argc < 2) {
        print_help();
        return 0;
    }

    std::string command = argv[1];
    if (command == "help" || command == "--help" || command == "-h") print_help();
    else if (command == "test") return run_manual_build();
    else if (command == "stress") return run_stress();
    else {
        std::cout << "Invalid command \"" << command << "\"\n\n";
        std::cout << "Try \"forge help\" \n\n";
    }


    return 0;
}
