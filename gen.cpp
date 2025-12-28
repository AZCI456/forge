// note that this gen algorithm is adapted from boiler plate code here
// Source: https://codeforces.com/blog/entry/61587
// Author: Neal Wu
#include <iostream>
#include <vector>
#include <random>

// Mersenne Twister algorithm - seeded by the system clock
std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

// Helper function for a random integer - inclusive
long long rand_int(long long min, long long max) {
    return std::uniform_int_distribution<long long>(min, max)(rng);
}

int main(int argc, char* argv[]) {
    //  pass a number to the generator as a seed
    // allowing you reproduce a specific "random" case if a bug arrises.
    if (argc > 1) {
        rng.seed(atoi(argv[1]));
    }

    // 1. Generate N (num test cases)
    // suggestion - keep low as bf.cpp is usually a slower algorithm (N=1
    int n = rand_int(1, 10);
    std::cout << n << "\n";

    // 2. change based on question
    for (int i = 0; i < n; ++i) {
        long long x = rand_int(-1000, 1000);
        long long y = rand_int(-1000, 1000);
        
        std::cout << x << " " << y << "\n";
    }
}