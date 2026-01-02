//
// Created by Aaron Coelho-Irani on 2/1/2026.
//

#include <ClipboardTools.h>

// this code is 100% AI adapted from the Internet boilerplate.
// was just a cool feature I wanted to try out - not much configurability that
// I can do on my end. Therefore, happy just using the boilerplate code

// cool to understand though and learn about different cross compatibility options

#include <iostream>
#include <string>

// Detect OS for cross-platform compatibility
#ifdef _WIN32
    #define POPEN _popen
    #define PCLOSE _pclose
    #define CLIP_CMD "clip"
#elif __APPLE__
    #define POPEN popen
    #define PCLOSE pclose
    #define CLIP_CMD "pbcopy"
#else // Linux
    #define POPEN popen
    #define PCLOSE pclose
    #define CLIP_CMD "xclip -selection clipboard"
#endif

void copyToClipboard(const std::string& text) {
    // Open a pipe (data funnel |) to the system clipboard command
    FILE* pipe = POPEN(CLIP_CMD, "w");

    if (!pipe) {
        std::cerr << "Failed to open clipboard pipe." << std::endl;
        return;
    }

    // Write the text to the pipe (stdin of the clipboard tool)
    fwrite(text.data(), 1, text.size(), pipe);

    // Close the pipe
    PCLOSE(pipe);

    std::cout << "Copied to clipboard!" << std::endl;
}
