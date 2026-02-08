#pragma once
#include <string>
#include <vector>
#include <random>

class RandomStreamGen {
public:
    RandomStreamGen(uint64_t seed = 12345);
    // generate stream of N strings. max_len <= 30
    std::vector<std::string> generate_stream(size_t N, size_t max_len = 30);
    // split positions in percents e.g., step_percent=5 => returns vector<size_t> of prefixes boundaries.
    static std::vector<size_t> split_positions(size_t total, size_t step_percent);
private:
    std::mt19937_64 rng;
    std::string alphabet;
};
