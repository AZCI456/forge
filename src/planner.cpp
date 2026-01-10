#include "Types.h"
#include <iostream>

// function constructs a dependency matrix on the compile task.
// Might be a bit overkill and then just using a simple for loop but wanted to try out multithreading.
// so that I could apply to future projects

std::vector<Task> create_plan(const FolderState& state) {

	std::vector<Task> plan;

	// the root of all tasks - compiling the program
	Task compile_task;
    compile_task.name = "compile";
    compile_task.command = "clang++ -std=c++17 " + state.sol_path.string() + " -o " + state.sol_path.stem().string();
    //std::cout << compile_task.command << "\n\n\n";
	// Add it to the plan
    plan.push_back(compile_task);

    // test files. 
    for (const auto& input_path : state.tests) {
        Task run_task;
        
        // eg "test1.in"
        run_task.name = input_path.filename().string();

        // Dependencies: It cannot start until "compile" is done.
        run_task.dependencies.emplace_back("compile"); // push_back but constructing the object in the container rather than copying

        // output filename:
        // eg "tests1.out"
        fs::path output_path(input_path); // copy constructor
        output_path.replace_filename("s_"+output_path.filename().string()); // s_ for sol generated - doesnt overwrite the provided files
        output_path.replace_extension(".out");

        // terminal command
        // eg. "./sol < tests1.in > tests1.out"
        run_task.command = "./sol < " + input_path.string() + " > " + output_path.string();
        // if answer exists 
        if ( state.valid_sols.count(input_path)){
            // Run diff to compare outputs
            // First try colordiff, fallback to regular diff if not available
            // Show the diff output (using || true to always show it), then check if files are identical
            std::string expected_output = state.valid_sols.at(input_path).string();
            // run_task.command += " && (colordiff -y " + output_path.string() + " " + expected_output + " 2>/dev/null || diff -y " + output_path.string() + " " + expected_output + " || true) && diff -q " + output_path.string() + " " + expected_output + " > /dev/null 2>&1";
            run_task.command += "&& diff -y " + output_path.string() + " " + expected_output + "";
        }


        plan.push_back(run_task);


    }

    return plan;
}