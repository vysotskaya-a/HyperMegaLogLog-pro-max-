#include "HyperLogLog.h"
#include "HashFuncGen.h"
#include <cmath>
#include <algorithm>

HyperLogLog::HyperLogLog(int B_, bool use64_) : B(B_), use64(use64_) {
    m = 1u << B;
    regs.assign(m, 0);
}

void HyperLogLog::reset() {
    std::fill(regs.begin(), regs.end(), 0);
}

inline int HyperLogLog::rho(uint64_t w, int max_bits) {
    // position of first 1-bit starting from MSB of w in at most max_bits
    if(w == 0) return max_bits + 1;
    // count leading zeros in w in 64-bit space, but we only consider max_bits
    int lz = __builtin_clzll(w);
    int leading = lz - (64 - max_bits);
    if(leading < 0) leading = 0;
    return leading + 1;
}

double HyperLogLog::alpha_m() const {
    if(m == 16) return 0.673;
    if(m == 32) return 0.697;
    if(m == 64) return 0.709;
    return 0.7213/(1.0 + 1.079/(double)m);
}

void HyperLogLog::add(const std::string &s) {
    uint64_t x = HashFuncGen::hash64(s);
    // use top B bits for index (so that remaining bits are used to compute rho)
    uint32_t idx = (uint32_t)(x >> (64 - B));
    uint64_t w = x << B; // remaining 64-B bits are left-aligned in 64-bit
    int rz = rho(w, 64 - B);
    if(rz > regs[idx]) regs[idx] = (uint8_t)std::min<int>(rz, 255);
}

double HyperLogLog::estimate() const {
    double inv_sum = 0.0;
    for(uint32_t i=0;i<m;++i) inv_sum += std::pow(2.0, - (int)regs[i]);
    double E = alpha_m() * m * m / inv_sum;
    // small-range correction (linear counting) — optional
    // count zeros
    int V = 0;
    for(uint32_t i=0;i<m;++i) if(regs[i] == 0) ++V;
    if(E <= (5.0/2.0) * m && V > 0) {
        // linear counting
        return m * std::log((double)m / (double)V);
    }
    // large-range correction can be added but skipped for simplicity
    return E;
}

size_t HyperLogLog::memory_bytes() const {
    return regs.size() * sizeof(uint8_t);
}
