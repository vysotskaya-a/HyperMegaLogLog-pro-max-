#include "HashFuncGen.h"

// FNV-1a 64-bit constants
static const uint64_t FNV_OFFSET_BASIS = 14695981039346656037ULL;
static const uint64_t FNV_PRIME = 1099511628211ULL;

uint64_t HashFuncGen::fnv1a_64(const std::string &s) {
    uint64_t h = FNV_OFFSET_BASIS;
    for(unsigned char c : s) {
        h ^= (uint64_t)c;
        h *= FNV_PRIME;
    }
    return h;
}

uint64_t HashFuncGen::hash64(const std::string &s) {
    // Future: could mix with std::hash or Murmur; for now FNV-1a 64-bit is fast and adequate.
    return fnv1a_64(s);
}
