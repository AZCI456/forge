#include <iostream>
#include <filesystem>
#include <cassert>

namespace fs = std::filesystem;

int main() {
    fs::path base_path{R"(/home/usera)"};
    fs::path target_path{R"(/home/usera/documents/doc.txt)"};

    // Calculate the relative path from base_path to target_path
    fs::path relative_path = fs::relative(target_path, base_path);

    std::cout << "Base Path: " << base_path << std::endl;
    std::cout << "Target Path: " << target_path << std::endl;
    std::cout << "Relative Path: " << relative_path << std::endl;

    // The output for relative_path will be "documents/doc.txt"
    assert(relative_path == fs::path("documents/doc.txt"));

    // Example with different base
    fs::path another_target{R"(/home/usera/downloads/file.zip)"};
    fs::path relative_to_another = fs::relative(another_target, target_path);
    std::cout << "Relative path from target to downloads: " << relative_to_another << std::endl;
    // Output will be "../../downloads/file.zip"
}
