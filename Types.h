#pragma once //  prevents copy pasting the struct code twice in one file on compilation

#include <vector>
#include <filesystem>
#include <unordered_map>

namespace fs = std::filesystem;


struct FolderState {

	fs::path op_sol; // optimised solution
	fs::path bf_sol; // brute force "
	std::vector<fs::path>  tests;
	// don't need the ordering functionality and low chance of hash collisions
	std::unordered_map<fs::path, fs::path> valid_sols;

	 // can't perform subsequent analysis without a solution file
	bool valid_build(){
		return ! op_sol.empty(); 
	}
};


struct Task {
	std::string name;
	std::string command;
	std::vector<std::string> dependencies;
};