

#include "../../include/InputTools.h"

#include <iostream> // cin/cout - get line inherited from istream (parent)
#include <fstream> // includes iostream but Imma keep it here just for clarity
#include <string>
#include <filesystem> // C++17 standard

// Command configuration structure - easily extensible for future flags
struct InputConfig {
    std::string cmd_to_answer;      // Command to switch from input to answer
    std::string cmd_next_case;      // Command to go to next test case
    std::string help_to_answer;     // Help text for switching to answer
    std::string help_next_case;     // Help text for going to next case
    std::string prompt_to_answer;   // Prompt text when entering input
    std::string prompt_next_case;   // Prompt text when entering output
    
    // Check if a line matches the command to switch to answer
    bool is_answer_command(const std::string& line) const {
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

// Create configuration based on flags
InputConfig create_config(bool copy_paste) {
    InputConfig config;
    
    if (copy_paste) {
        config.cmd_to_answer = "--";
        config.cmd_next_case = "++";
        config.help_to_answer = "Type '" + config.cmd_to_answer + "' to switch to Answer.";
        config.help_next_case = "Type '" + config.cmd_next_case + "' to save and start Next Case.";
        config.prompt_to_answer = " (Type '" + config.cmd_to_answer + "' for Output):";
        config.prompt_next_case = " (Type '" + config.cmd_next_case + "' for Next):";
    } else {
        config.cmd_to_answer = "";  // Empty means Enter key
        config.cmd_next_case = "";  // Empty means Enter key
        config.help_to_answer = "Press Enter to switch to Answer.";
        config.help_next_case = "Press Enter to save and start Next Case.";
        config.prompt_to_answer = " (Press Enter for Output):";
        config.prompt_next_case = " (Press Enter for Next):";
    }
    
    return config;
}

// Helper to find the next available case number
int getNextCaseNumber() {
    int i = 1;
    while (std::filesystem::exists(std::to_string(i) + ".in")) {
        i++;
    }
    return i;
}

int handle_input_tests(bool copy_paste) {
    int caseNum = getNextCaseNumber();
    std::string line;
    
    // Create configuration based on flags
    InputConfig config = create_config(copy_paste);

    std::cout << "=== Forge Test Creator ===\n";
    std::cout << "Directly writing to individual files.\n";
    std::cout << config.help_to_answer << "\n";
    std::cout << config.help_next_case << "\n";
    std::cout << "Press CTRL+D to Quit.\n\n";

    while (true) { // runs till the user quits

        // --- STEP 1: INPUT ---
        std::string inFileName = std::to_string(caseNum) + ".in";
        std::ofstream inFile(inFileName);

        if (!inFile) { 
            std::cerr << "Error creating " << inFileName << std::endl; 
            return 1; 
        }

        std::cout << "[Case " << caseNum << "] Enter Input" << config.prompt_to_answer << "\n";

        while (std::getline(std::cin, line)) {
            if (config.is_answer_command(line)) {
                break; // Stop writing to .in, move to .out
            }
            // Direct write to file
            inFile << line << "\n";
        }
        inFile.close(); // Save and close immediately

        // Handle CTRL+D (EOF) during input
        if (std::cin.eof()) {
            std::cout << "\n>> EOF detected. " << inFileName << " deleted. Exiting.\n";
            // Delete the partial file
            std::filesystem::remove(inFileName);
            return 0;
        }

        // --- STEP 2: OUTPUT ---
        std::string outFileName = std::to_string(caseNum) + ".out";
        std::ofstream outFile(outFileName);

        if (!outFile) { 
            std::cerr << "Error creating " << outFileName << std::endl; 
            return 1; 
        }

        std::cout << "[Case " << caseNum << "] Enter Expected Output" << config.prompt_next_case << "\n";

        while (std::getline(std::cin, line)) {
            if (config.is_next_command(line)) {
                break; // Stop writing to .out, restart loop
            }
            // Direct write to file
            outFile << line << "\n";
        }
        outFile.close(); // Save and close immediately

        // Handle CTRL+D (EOF) during output
        if (std::cin.eof()) {
            std::cout << "\n>> EOF detected. " << outFileName << " deleted. Exiting.\n";
            std::filesystem::remove(outFileName);
            break;
        }

        std::cout << ">> Case " << caseNum << " Saved.\n\n";
        caseNum++;
    }

    return 0;
}