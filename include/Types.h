#pragma once //  prevents copy pasting the struct code twice in one file on compilation

#include <vector>
#include <filesystem>
#include <unordered_map>

namespace fs = std::filesystem;


struct FolderState {

	fs::path sol_path; // final solution
	fs::path bf_path; // brute force
	fs::path generator_path;
	std::vector<fs::path>  tests;
	// don't need the ordering functionality and low chance of hash collisions
	std::unordered_map<fs::path, fs::path> valid_sols;

	 // can't perform subsequent analysis without a solution file
	[[nodiscard]] bool valid_build() const{
		return ! sol_path.empty() && ! tests.empty() && ! valid_sols.empty();
	}

	[[nodiscard]] bool valid_stress() const{
		return ! sol_path.empty() && ! bf_path.empty() && ! generator_path.empty();
	}
};


struct Task {
	std::string name;
	std::string command;
	std::vector<std::string> dependencies;
};

// Command configuration structure - easily extensible for future flags
struct InputConfig {
	std::string cmd_to_answer;      // Command to switch from input to answer
	std::string cmd_next_case;      // Command to go to next test case
	std::string help_to_answer;     // Help text for switching to answer
	std::string help_next_case;     // Help text for going to next case
	std::string prompt_to_answer;   // Prompt text when entering input
	std::string prompt_next_case;   // Prompt text when entering output

	// Check if a line matches the command to switch to answer
	[[nodiscard]] bool is_answer_command(const std::string& line) const {
		if (cmd_to_answer.empty()) {
			return line.empty(); // Enter key mode
		}
		return line == cmd_to_answer; // String command mode
	}

	// Check if a line matches the command to go to next case
	bool is_next_command(const std::string& line) const {
		if (cmd_next_case.empty()) {
			return line.empty(); // Enter key mode
		}
		return line == cmd_next_case; // String command mode
	}
};
