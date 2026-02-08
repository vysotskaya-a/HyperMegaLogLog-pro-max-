#pragma once
#include "HyperLogLog.h"
#include <unordered_set>
#include <string>

// Improved HLL:
// - use exact set for small cardinalities (sparse) until threshold
// - identical register logic otherwise (64-bit hash), same alpha_m
class HyperLogLogImproved : public HyperLogLog {
public:
    HyperLogLogImproved(int B, bool use64 = true, size_t sparse_threshold = 4096);
    void add(const std::string &s);
    double estimate() const;
    void reset();
    size_t get_sparse_size() const;
private:
    std::unordered_set<std::string> sparse_set;
    size_t sparse_threshold;
    bool in_sparse() const;
};
