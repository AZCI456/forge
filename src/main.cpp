#include <iostream>
#include <string>
#include <vector>

#include "CLI.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        print_help();
        return 0;
    }

    // Convert argv to a friendlier container
    std::vector<std::string> args;
    args.reserve(static_cast<size_t>(argc - 1));
    for (int i = 1; i < argc; ++i) {
        args.emplace_back(argv[i]);
    }

    const std::string command = args.front();

    // Extract flags (everything after the command)
    std::vector<std::string> command_flags;
    if (args.size() > 1) {
        command_flags.assign(args.begin() + 1, args.end());
    }

    return handle_command(command, command_flags);
}
