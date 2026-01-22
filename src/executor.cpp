#include <fstream>
#include <iostream>
#include "Types.h"
#include "Colours.h"
#include <filesystem>


namespace fs = std::filesystem; // alias for brevity


// this coudl be a simple 5 liner but most is there so that the user understands where they are in the execution flow
bool run_program(const std::vector<Task> & tasks){

	std::cout << "COMPILING [TASKS]...\n\n";

	bool all_passed = true;
	for (const Task & task: tasks){

		// print the task name
		std::cout << "TESTING: " << task.name << "...";

		// then the result
		if(std::system(task.command.c_str())) {
			std::cout << colours::RED << "[FAILED]\n\n" << colours::RESET;
			all_passed = false;
			//return false; // early stop - uncomment all past to compile everything regardless of failure
		}
		else { // here in case you want to uncomment all past
			std::cout << colours::GREEN << "[SUCCESSFUL]\n\n" << colours::RESET;
		}

	}
	//return true;
	return all_passed;

}