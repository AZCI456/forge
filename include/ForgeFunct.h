#pragma once

#include "Types.h"
#include <filesystem>

FolderState parse_directory(const std::filesystem::path& current_directory);

std::vector<Task> create_plan(const FolderState& state);

bool run_program(const std::vector<Task> & tasks);
