#include "ExactCounter.h"

size_t ExactCounter::count_unique_prefix(const std::vector<std::string>& stream, size_t len) {
    std::unordered_set<std::string> s;
    s.reserve(len * 2);
    for(size_t i=0;i<len && i<stream.size();++i) s.insert(stream[i]);
    return s.size();
}
