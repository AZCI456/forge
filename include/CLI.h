#pragma once

#include <string>
#include <vector>

// High-level command handling for the Forge CLI.
// Keeps main.cpp as a thin wrapper so we can iterate on commands quickly.

// Entry point for handling a parsed command + its flags.
// Returns 0 on success and -1 on failure (or other non‑zero codes).
int  handle_command(const std::string& command,
                   const std::vector<std::string>& command_flags);

bool handle_EOF(std::string file_name);


// User-facing help and flag docs (also used by main when argc < 2).
int print_help();
int print_flags();


