#pragma once

#include "Types.h"
#include <filesystem>

FolderState parse_directory(const std::filesystem::path& current_directory, const fs::path & test_cases = ".");

std::vector<Task> create_plan(const FolderState& state);

bool run_program(const std::vector<Task> & tasks);
