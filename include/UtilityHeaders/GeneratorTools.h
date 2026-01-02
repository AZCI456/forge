#pragma once
#include <filesystem>
#include "../Types.h"


void prepareDirectory(const std::filesystem::path & folder);

void gen_tests();

// Generate and test one fuzzer test case, returns true if test passed
bool generate_and_test_one(int test_num, const FolderState& state);

// Run fuzzer: generate and test incrementally, breaking on first failure
int run_fuzzer(const FolderState& state);

//
// class GeneratorTools {
// };


