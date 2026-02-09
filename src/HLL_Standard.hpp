#pragma once
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include "MurmurHash3.hpp"

class HyperLogLogStd {
private:
    int b;
    int m;
    double alphaMM;
    std::vector<int> registers;

    double get_alpha(int m_val) {
        switch (m_val) {
        case 16: return 0.673;
        case 32: return 0.697;
        case 64: return 0.709;
        default: return 0.7213 / (1.0 + 1.079 / m_val);
        }
    }

    uint8_t count_leading_zeros(uint32_t val) {
        if (val == 0) return 32;
        uint8_t zeros = 0;
        while ((val & 0x80000000) == 0) {
            zeros++;
            val <<= 1;
        }
        return zeros + 1;
    }

public:
    HyperLogLogStd(int b_bits) : b(b_bits), m(1 << b_bits) {
        alphaMM = get_alpha(m) * m * m;
        registers.resize(m, 0);
    }

    void add(const std::string& s) {
        uint32_t hash;
        MurmurHash3_x86_32(s.c_str(), s.length(), 42, &hash);
        
        uint32_t index = hash >> (32 - b);
        uint32_t value = hash << b; 
        uint8_t rank = count_leading_zeros(value);
        
        if (rank > registers[index]) {
            registers[index] = rank;
        }
    }

    double count() const {
        double sum = 0.0;
        for (int val : registers) {
            sum += std::pow(2.0, -val);
        }
        double E = alphaMM / sum;

        if (E <= 2.5 * m) {
            int V = 0;
            for (int val : registers) {
                if (val == 0) V++;
            }
            if (V > 0) {
                E = m * std::log((double)m / V);
            }
        } else if (E > std::pow(2.0, 32) / 30.0) {
            E = -std::pow(2.0, 32) * std::log(1.0 - E / std::pow(2.0, 32));
        }
        return E;
    }

    size_t memory_usage() const {
        return registers.capacity() * sizeof(int);
    }
};