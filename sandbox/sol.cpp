#include <iostream>

int main() {
    int n;
    std::cin >> n;
    
    // Intentional Bug: Fails if input is 42
    if (n == 42) {
        std::cout << "99999" << std::endl; 
    } else {
        std::cout << (n + 1) << std::endl;
    }
    return 0;
}
