

#include "../../include/UtilityHeaders/InputTools.h"

#include <iostream> // cin/cout - get line inherited from istream (parent)
#include <fstream> // includes iostream but Imma keep it here just for clarity
#include <string>
#include <filesystem> // C++17 standard

#include <Types.h>


// Create configuration based on flags
// No third argument, use Enter key mode
InputConfig create_in_config(bool manual_mode) {
    InputConfig in_config;
    
    if (manual_mode) {
        in_config.cmd_to_answer = "--";
        in_config.cmd_next_case = "++";
        in_config.help_to_answer = "Type '" + in_config.cmd_to_answer + "' to switch to Answer.";
        in_config.help_next_case = "Type '" + in_config.cmd_next_case + "' to save and start Next Case.";
        in_config.prompt_to_answer = " (Type '" + in_config.cmd_to_answer + "' for Output):";
        in_config.prompt_next_case = " (Type '" + in_config.cmd_next_case + "' for Next):";
    } else {
        in_config.cmd_to_answer = "";  // Empty means Enter key
        in_config.cmd_next_case = "";  // Empty means Enter key
        in_config.help_to_answer = "Press Enter to switch to Answer.";
        in_config.help_next_case = "Press Enter to save and start Next Case.";
        in_config.prompt_to_answer = " (Press Enter for Output):";
        in_config.prompt_next_case = " (Press Enter for Next):";
    }
    
    return in_config;
}

// Helper to find the next available case number - iterate through directory and find the next available file name
int getNextCaseNumber() {
    int i = 1;
    while (std::filesystem::exists(std::to_string(i) + ".in")) {
        i++;
    }
    return i;
}

// Runs the interactive console loop for the Forge Test Creator.
int handle_input_tests(const std::vector<std::string> & flags) {
    /* note this will start overwriting files if you randomly delete one in your directory
    move to while loop if you want to prevent that from happening (at performance cost
    of parsing your entire directory every single time */

    // first check if "flags" is not empty for short circuiting to prevent a segfault
    bool manual_mode = ! flags.empty() && flags[0] == "-m";

    int caseNum = getNextCaseNumber();
    std::string line;
    
    // Create configuration based on flags
    InputConfig in_config = create_in_config(manual_mode);

    std::cout << "=== Forge Test Creator ===\n";
    std::cout << "Directly writing to individual files.\n";
    std::cout << in_config.help_to_answer << "\n";
    std::cout << in_config.help_next_case << "\n";
    std::cout << "Press CTRL+D to Quit.\n\n";

    while (true) { // runs till the user quits

        // --- STEP 1: INPUT ---
        std::string inFileName = std::to_string(caseNum) + ".in";
        std::ofstream inFile(inFileName);

        if (!inFile) { 
            std::cerr << "Error creating " << inFileName << std::endl; 
            return 1; 
        }

        std::cout << "[Case " << caseNum << "] Enter Input" << in_config.prompt_to_answer << "\n";

        while (std::getline(std::cin, line)) {
            if (in_config.is_answer_command(line)) {
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

        std::cout << "[Case " << caseNum << "] Enter Expected Output" << in_config.prompt_next_case << "\n";

        while (std::getline(std::cin, line)) {
            if (in_config.is_next_command(line)) {
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