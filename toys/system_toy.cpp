#include <iostream>
#include <cstdlib> // Required for std::system

int main() {
    // Try a command that works
    int result1 = std::system("ls -l");
    std::cout << "ls -l returned: " << result1 << std::endl;

    // Try a command that fails (compiling a non-existent file)
    int result2 = std::system("g++ fake_file.cpp");
    std::cout << "g++ returned: " << result2 << std::endl;

    return 0;
}