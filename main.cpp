#include "Types.h"
#include "ForgeFunct.h"
#include <iostream>
#include <filesystem>

// Declaration of the function in directory_parser.cpp

int main() {
    auto state = parse_directory(".");
    if (state.valid_build()) {
        std::cout << "Success!" << std::endl;
    }
    else return -1;

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

    if (run_program(tasks)) std::cout << "ALL TASKS SUCCESSFULLY COMPILED\n\n";
    else {
        std::cout << "AT LEAST ONE TASK FAILED... ABORTING!\n";
        return -1;
    }
    

    return 0;
}