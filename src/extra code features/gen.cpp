#include <iostream>
#include <random>
#include <chrono>

// Source: https://codeforces.com/blog/entry/61587
std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

// for strings use this in the while loop 
// static_cast<char>() range (A–Z): 65 to 90,  (a–z): 97 to 122.
// binary string     std::cout << rand_int(0,1);


long long rand_int(long long min, long long max) {
    return std::uniform_int_distribution<long long>(min, max)(rng);
}

int main(int argc, char* argv[]) {
    if (argc > 1) rng.seed(std::atoll(argv[1]));

    // 1. Generate Number of Test Cases
    int t = rand_int(10, 20);
    std::cout << t << "\n";

    while (t--) {
        // 2. Generation Logic
        // logic here
        // eg.

        std::cout << static_cast<char>(rand_int(65,66));
    }
    return 0;
}