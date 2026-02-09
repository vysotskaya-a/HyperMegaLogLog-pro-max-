#pragma once
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include "MurmurHash3.hpp"

class HyperLogLogImp {
private:
    int b;
    int m;
    double alphaMM;
    std::vector<uint8_t> registers;

    double get_alpha(int m_val) {
        switch (m_val) {
        case 16: return 0.673;
        case 32: return 0.697;
        case 64: return 0.709;
        default: return 0.7213 / (1.0 + 1.079 / m_val);
        }
    }

    uint8_t count_leading_zeros_64(uint64_t val) {
        if (val == 0) return 64;
        uint8_t zeros = 0;
        while ((val & 0x8000000000000000ULL) == 0) {
            zeros++;
            val <<= 1;
        }
        return zeros + 1;
    }

public:
    HyperLogLogImp(int b_bits) : b(b_bits), m(1 << b_bits) {
        alphaMM = get_alpha(m) * m * m;
        registers.resize(m, 0);
    }

    void add(const std::string& s) {
        uint64_t hash[2];
        MurmurHash3_x64_128(s.c_str(), s.length(), 42, hash);
        
        uint64_t h64 = hash[0]; 
        uint32_t index = h64 >> (64 - b);
        
        uint64_t value = h64 << b; 
        uint8_t rank = count_leading_zeros_64(value);
        
        if (rank > registers[index]) {
            registers[index] = rank;
        }
    }

    double count() const {
        double sum = 0.0;
        for (uint8_t val : registers) {
            sum += std::pow(2.0, -val);
        }
        double E = alphaMM / sum;

        if (E <= 2.5 * m) {
            int V = 0;
            for (uint8_t val : registers) {
                if (val == 0) V++;
            }
            if (V > 0) {
                E = m * std::log((double)m / V);
            }
        }
        
        return E;
    }

    size_t memory_usage() const {
        return registers.capacity() * sizeof(uint8_t);
    }
};