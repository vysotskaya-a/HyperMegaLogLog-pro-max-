#pragma once
#include <string>
#include <vector>
#include <random>
#include <algorithm>

class RandomStreamGen {
private:
    std::mt19937 rng;
    std::uniform_int_distribution<int> length_dist;
    std::uniform_int_distribution<int> char_dist;
    const std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-";

public:
    RandomStreamGen(unsigned int seed = 42) : rng(seed), length_dist(1, 30), char_dist(0, 62) {}

    std::string next_string() {
        int len = length_dist(rng);
        std::string s;
        s.reserve(len);
        for (int i = 0; i < len; ++i) {
            s += chars[char_dist(rng)];
        }
        return s;
    }

    std::vector<std::string> generate_batch(size_t count) {
        std::vector<std::string> batch;
        batch.reserve(count);
        for (size_t i = 0; i < count; ++i) {
            batch.push_back(next_string());
        }
        return batch;
    }
};