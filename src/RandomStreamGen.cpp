#include "RandomStreamGen.h"
#include <algorithm>
#include <random>

RandomStreamGen::RandomStreamGen(uint64_t seed) : rng(seed) {
    alphabet = "";
    for(char c='a'; c<='z'; ++c) alphabet.push_back(c);
    for(char c='A'; c<='Z'; ++c) alphabet.push_back(c);
    for(char c='0'; c<='9'; ++c) alphabet.push_back(c);
    alphabet.push_back('-');
}

std::vector<std::string> RandomStreamGen::generate_stream(size_t N, size_t max_len) {
    std::vector<std::string> res;
    res.reserve(N);
    std::uniform_int_distribution<int> len_dist(1, (int)std::min<size_t>(30, max_len));
    std::uniform_int_distribution<size_t> idx_dist(0, alphabet.size()-1);
    for(size_t i=0;i<N;++i) {
        int L = len_dist(rng);
        std::string s;
        s.reserve(L);
        for(int j=0;j<L;++j) s.push_back(alphabet[idx_dist(rng)]);
        res.push_back(std::move(s));
    }
    return res;
}

std::vector<size_t> RandomStreamGen::split_positions(size_t total, size_t step_percent) {
    std::vector<size_t> pos;
    if(step_percent == 0) return pos;
    for(size_t p = step_percent; p <= 100; p += step_percent) {
        size_t k = (total * p) / 100;
        if(k==0) k = 1;
        if(k > total) k = total;
        pos.push_back(k);
    }
    return pos;
}
