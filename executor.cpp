#include <cstdlib>
#include <iostream>
#include "Types.h"

// this coudl be a simple 5 liner but most is there so that the user understands where they are in the execution flow
bool run_program(const std::vector<Task> & tasks){

	std::cout << "COMPILING [TASKS]..." << '\n';

	for (Task task: tasks){

		if(std::system(task.command.c_str())) ; // function requires c style string
		else std::cout << task.name << " SUCCESSFULLY EXECUTED\n";
		
	}
	return true;

}