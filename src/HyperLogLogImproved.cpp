#include "HyperLogLogImproved.h"
#include <algorithm>

HyperLogLogImproved::HyperLogLogImproved(int B, bool use64, size_t sparse_threshold_)
: HyperLogLog(B, use64), sparse_threshold(sparse_threshold_) {
    sparse_set.reserve(std::min<size_t>(sparse_threshold, 4096));
}

bool HyperLogLogImproved::in_sparse() const {
    return sparse_set.size() > 0 && sparse_set.size() <= sparse_threshold;
}

void HyperLogLogImproved::add(const std::string &s) {
    if(in_sparse()) {
        sparse_set.insert(s);
        if(sparse_set.size() > sparse_threshold) {
            // move elements to registers
            for(const auto &x : sparse_set) HyperLogLog::add(x);
            sparse_set.clear();
        }
    } else {
        // if currently empty but threshold>0, try to insert first elements into sparse set
        if(sparse_set.size() < sparse_threshold && sparse_set.size() == 0) {
            sparse_set.insert(s);
        } else {
            HyperLogLog::add(s);
        }
    }
}

double HyperLogLogImproved::estimate() const {
    if(in_sparse()) return (double)sparse_set.size();
    return HyperLogLog::estimate();
}

void HyperLogLogImproved::reset() {
    HyperLogLog::reset();
    sparse_set.clear();
}

size_t HyperLogLogImproved::get_sparse_size() const { return sparse_set.size(); }
