#include <cstdlib>
#include <iostream>
#include "types.h"
#include "colours.h"


// this coudl be a simple 5 liner but most is there so that the user understands where they are in the execution flow
bool run_program(const std::vector<Task> & tasks){

	std::cout << "COMPILING [TASKS]..." << '\n';

	for (Task task: tasks){

		// print the task name
		std::cout << task.name << "...";

		// then the result
		// implementation note - break early so that when many test cases can see where failed
		if(std::system(task.command.c_str())) std::cout  << RED << "[FAILED]\n\n" << RESET; // function requires c style string
		else std::cout << GREEN << "[SUCCESSFUL]\n\n" << RESET;
		
	}
	return true;

}