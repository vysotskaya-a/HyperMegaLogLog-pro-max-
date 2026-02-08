#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "RandomStreamGen.h"
#include "ExactCounter.h"
#include "HyperLogLog.h"
#include "HyperLogLogImproved.h"

int main() {
    // =========================
    // НАСТРОЙКИ ЭКСПЕРИМЕНТА
    // =========================
    const size_t STREAM_SIZE = 1'000'000;
    const int STEP_PERCENT = 5;
    const int B = 12;
    const uint64_t SEED = 42;
    const std::string OUT_FILE = "results.csv";

    std::cout << "Generating stream...\n";
    RandomStreamGen gen(SEED);
    auto stream = gen.generate_stream(STREAM_SIZE);

    auto positions = RandomStreamGen::split_positions(STREAM_SIZE, STEP_PERCENT);

    HyperLogLog hll(B);
    HyperLogLogImproved hll_imp(B);

    std::ofstream out(OUT_FILE);
    out << "step,processed,true_unique,hll_est,hll_improved_est\n";

    int step = 0;
    for (size_t pos : positions) {
        step++;

        hll.reset();
        hll_imp.reset();

        for (size_t i = 0; i < pos; ++i) {
            hll.add(stream[i]);
            hll_imp.add(stream[i]);
        }

        size_t true_cnt = ExactCounter::count_unique_prefix(stream, pos);
        double est_hll = hll.estimate();
        double est_imp = hll_imp.estimate();

        out << step << ","
            << pos << ","
            << true_cnt << ","
            << est_hll << ","
            << est_imp << "\n";

        std::cout << "Step " << step
                  << " | processed=" << pos
                  << " | true=" << true_cnt
                  << " | hll=" << (uint64_t)est_hll
                  << " | improved=" << (uint64_t)est_imp
                  << "\n";
    }

    out.close();
    std::cout << "\nSaved results to results.csv\n";
    return 0;
}
