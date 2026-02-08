#pragma once
#include <string>
#include <unordered_set>
#include <vector>

class ExactCounter {
public:
    // count unique in prefix of stream (0..len-1)
    static size_t count_unique_prefix(const std::vector<std::string>& stream, size_t len);
};
