#pragma once
#include <vector>
#include <string>
#include <cstdint>

class HyperLogLog {
public:
    // B = number of index bits => m = 2^B registers
    HyperLogLog(int B, bool use64 = true);
    void add(const std::string &s);
    double estimate() const;
    size_t memory_bytes() const;
    void reset();
private:
    int B;
    uint32_t m;
    std::vector<uint8_t> regs;
    bool use64;
    double alpha_m() const;
    static inline int rho(uint64_t w, int max_bits);
};
