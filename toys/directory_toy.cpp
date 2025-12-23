#include <iostream>
#include <filesystem>
//#include <fstream> // Required for file content I/O

#include <string>

namespace fs = std::filesystem; // Use an alias for convenience

// compile with c++17 upwards for filesystem namespace
// clang++ directory_toy.cpp -std=gnu++17 

#define CODE_EXT ".cpp"
#define INPUT_EXT ".in"
#define OUTPUT_EXT ".out"



void valid_file(const std::string & filename, const std::string & extension){
	if (! filename.size()) return; 

	if (extension == CODE_EXT) {
	    std::cout << "code file";
	}
	else if (extension == INPUT_EXT) {
	    std::cout << "input file";
	}
	else if (extension == OUTPUT_EXT) {
	    std::cout << "output file";
	}


	std::cout << std::endl;
} 

int main(){

	std::string filename, extension;

	for (const auto& entry : fs::directory_iterator(".")) {
		filename = entry.path().filename().string();
		extension = entry.path().extension().string();
    	valid_file(filename, extension);
	}
	return 0;
}	