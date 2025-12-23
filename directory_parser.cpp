// compile testing 
// clang++ directory_parser.cpp -o directory_parser -std=gnu++17 && ./directory_parser

#include <iostream>
#include <filesystem>
#include <vector>
#include <string>

// for the FolderState Struct
#include "Types.h"

namespace fs = std::filesystem; // alias for brevity


// put whatever you call you main coding file here
#define SOLUTION_NAME "sol.cpp" // include .cpp
#define BRUTE_NAME "bf.cpp" 
#define TEST_EXT ".in" 
#define VSOL_EXT ".ans" 

// understand better then implement - supposedly more efficent
// inline constexpr std::string_view SOLUTION_NAME = "sol.cpp";
// inline constexpr std::string_view BRUTE_NAME    = "bf.cpp";
// inline constexpr std::string_view TEST_EXT      = ".in";
// inline constexpr std::string_view VSOL_EXT      = ".in";


// optimisation: remove string conversions in directory parser to avoid unnessary heap allocation
void parse_file(fs::path file_path, FolderState & folderState){

	const auto filename = file_path.filename();

	// lazy evalution if solution file
	if (filename == SOLUTION_NAME) {
		folderState.op_sol = file_path; 
		return;
	}
	else if (filename == BRUTE_NAME){
		folderState.bf_sol = file_path;
		return;
	} 
	
	// then test solution
	const auto ext = file_path.extension();

	if (ext == TEST_EXT){
		folderState.tests.push_back(file_path);
		// see if solution file given as well
		fs::path ans_file = fs::path(file_path).replace_extension(VSOL_EXT);
		//std::cout << ans_file << "\n\n";
		if (fs::exists(ans_file)){
			//std::cout << ans_file << "Exists" << "\n\n\n";
			folderState.valid_sols.insert({file_path, ans_file});
		} 

		return;
	} 

	// reaches this point then invalid file

}

FolderState parse_directory(const fs::path & current_directory){

	FolderState folderState;

	// reference files in current "." directory
	for (const auto & file : fs::directory_iterator(current_directory)) { 
    	parse_file(file.path(), folderState);
	}

	// check if successful
	if (!folderState.valid_build()) {
        std::cerr << "[ERROR] " << SOLUTION_NAME << " file not found\n";
        // build failed 
        //#warning ensure this is caught in the main function yes
        return folderState;
    }

    std::cout << "[FORGE] Project Scan Complete:\n";
    std::cout << "  Solution: " << folderState.op_sol.filename() << "\n";
    std::cout << "  BF Approach: " << (folderState.bf_sol.empty() ? "N":"Y") << "\n";
    // GENERATOR FEATURE STUB
    //std::cout << "  Generator: " << (folderState.generator_path.empty() ? "None" : "Found") << "\n";
    std::cout << "  Manual Tests: " << folderState.tests.size() << " found.\n";

    return folderState;



}