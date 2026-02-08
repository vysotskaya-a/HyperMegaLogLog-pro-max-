#pragma once
#include <string>
#include <cstdint>

class HashFuncGen {
public:
    // FNV-1a 64-bit
    static uint64_t fnv1a_64(const std::string &s);
    // wrapper: produce 64-bit hash value
    static uint64_t hash64(const std::string &s);
};
