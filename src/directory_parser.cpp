#include <iostream>
#include <filesystem>
#include <vector>
#include <string>

#include "Colours.h"
#include "Types.h"
#include "Constants.h"

namespace fs = std::filesystem; // alias for brevity


void parse_file(const fs::path& file_path, FolderState & folderState){

	const auto filename = file_path.filename();

	// lazy evalution if solution file
	if (filename == config::SOLUTION_NAME) {
		folderState.sol_path = file_path;
		return;
	}
	else if (filename == config::BRUTE_NAME){
		folderState.bf_path = file_path;
		return;
	}
	else if (filename == config::GENERATOR_NAME) {
		folderState.generator_path = file_path;
		return;
	}
	
	// if here test solutions
	const auto ext = file_path.extension();

	if (ext == config::TEST_EXT){
		folderState.tests.push_back(file_path);
		// see if solution file given as well
		fs::path ans_file = fs::path(file_path).replace_extension(config::
			VSOL_EXT);
		//std::cout << ans_file << "\n\n";
		if (fs::exists(ans_file)){
			//std::cout << ans_file << "Exists" << "\n\n\n";
			folderState.valid_sols.insert({file_path, ans_file});
		} 

		return;
	} 

	// reaches this point then invalid file

}


// check if successful in the main calling function
// default is just the current directory
FolderState parse_directory(const fs::path & current_directory, const fs::path & test_cases = "."){

	FolderState folderState;

	// reference files in current "." directory
	for (const auto & file : fs::directory_iterator(current_directory)) { 
    	parse_file(file.path(), folderState);
	}
	if (test_cases != current_directory) {
		for (const auto & test : fs::directory_iterator(test_cases)) {
			parse_file(test.path(), folderState);
		}
	}

	// check if successful

	std::string SUCCESS = colours::GREEN+"Y";
	std::string FAIL = colours::RED+"N";

    std::cout << "[FORGE] Project Scan Complete:\n";
    std::cout << "  Solution: " << (folderState.sol_path.empty() ? FAIL:SUCCESS) << colours::RESET << "\n"; // otherwise can show the filename
    std::cout << "  BF Approach: " << (folderState.bf_path.empty() ? FAIL:SUCCESS) << colours::RESET << "\n";
    // GENERATOR FEATURE STUB
    std::cout << "  Generator: " << (folderState.generator_path.empty() ? FAIL:SUCCESS) << colours::RESET << "\n";
    std::cout << "  Manual Tests: " << folderState.tests.size() << " found.\n";

    return folderState;



}