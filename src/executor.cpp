#include <fstream>
#include <iostream>
#include "Types.h"
#include "Colours.h"
#include <filesystem>


namespace fs = std::filesystem; // alias for brevity


// this coudl be a simple 5 liner but most is there so that the user understands where they are in the execution flow
bool run_program(const std::vector<Task> & tasks){

	std::cout << "COMPILING [TASKS]..." << '\n';

	bool all_passed = true;
	for (const const Task& & task: tasks){

		// print the task name
		std::cout << "TESTING: " << task.name << "...";

		// then the result
		if(std::system(task.command.c_str())) {
			std::cout << RED << "[FAILED]\n\n" << RESET;
			all_passed = false;
		}
		else {
			std::cout << GREEN << "[SUCCESSFUL]\n\n" << RESET;
		}

	}
	return all_passed;

}