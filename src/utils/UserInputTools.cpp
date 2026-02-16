#include "../../include/UtilityHeaders/UserInputTools.h"
#include <iostream>
#include <fstream>
#include <string>

// void appendToFile(const std::string& filename) {
//     std::ofstream outfile(filename, std::ios_base::app);
//     std::string line;
//
//     // This loop runs until it hits EOF (Control+D)
//     while (std::getline(std::cin, line)) {
//         outfile << line << "\n";
//
//         // This ensures the terminal knows the line is 'done'
//         // and keeps the cursor behaviour predictable
//         std::cout << std::flush;
//     }
// }

// note as with a lot of the other utility code this is AI generated
// I have gone through it and I understand what every line does
// I don't believe there will be any issues however if you notice anything, please do contact me'
#include <iostream>
#include <fstream>
#include <string>

void simpleAppend(const std::string& filename) {
    // Open in append mode
    std::ofstream outfile(filename, std::ios_base::app);

    if (!outfile.is_open()) {
        std::cerr << "Error: Could not open " << filename << std::endl;
        return;
    }

    std::string line;
    std::cout << "(Recording... Press Ctrl+D to stop)\n> " << std::flush;

    // This loop stays alive until you hit Ctrl+D
    while (std::getline(std::cin, line)) {
        outfile << line << "\n"; // Write to your file
        outfile.flush();         // Save immediately

        // The fix: Manual cursor feedback
        // is the only way to have the cursor jump to the New line instantly.
        // not sure on the style but it's the only way to stop the PAC errors from happening
        std::cout << "> " << std::flush;
    }

    std::cout << "\n[Done]" << std::endl;
}

int printFileContent(const std::string& filename) {
    std::ifstream infile(filename); // Open for reading

    if (!infile) {
      std::cout << "File \"" << filename << "\"not found" << std::endl;
    }
    else if (infile.is_open()) {
        // This prints the entire file buffer to cout in one go
        std::cout << infile.rdbuf();
    }
    else {
        std::cerr << "Unexpected issue with showing file contents: forward to developer "
                     "or inspect in terminal"
        << std::endl;
        return -1;
    }

    // Visually close out the file that was started in the parent function
    std::cout << "--------------" << std::endl;

    return 0;
}