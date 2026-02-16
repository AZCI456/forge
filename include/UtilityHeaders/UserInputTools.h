#pragma once

#include <string>

// this one is void as there are not much errors that can happen with it
// the only error that I can think of is maybe trying to write to a folder that you don't have permission to
// but these errors are pretty explicit when thrown so I'm not coding try catch for it
void simpleAppend(const std::string& filename);

// this one can fail if you try and open a file that doesn't exist
// Therefore, this is handled in the function
int printFileContent(const std::string& filename);