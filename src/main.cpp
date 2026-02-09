#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <iomanip>
#include "Generators.hpp"
#include "HLL_Standard.hpp"
#include "HLL_Improved.hpp"

struct Stats {
    double mean;
    double std_dev;
    double true_val;
};

void run_experiment_std(int b, int total_elements, int steps, int runs, const std::string& filename) {
    std::ofstream file(filename);
    file << "step,processed,true_cardinality,est_mean,est_stddev,memory_bytes\n";

    int step_size = total_elements / steps;
    
    std::vector<std::vector<double>> run_results(steps + 1);
    std::vector<double> true_counts(steps + 1);
    size_t mem_usage = 0;

    for (int r = 0; r < runs; ++r) {
        RandomStreamGen gen(42 + r);
        HyperLogLogStd hll(b);
        std::set<std::string> unique_elements;
        
        mem_usage = hll.memory_usage();

        for (int s = 0; s <= steps; ++s) {
            int target_size = s * step_size;
            
            int needed = target_size - (s == 0 ? 0 : (s-1)*step_size);
            if (s == 0) needed = 0;

            for(int k=0; k<needed; ++k) {
                std::string str = gen.next_string();
                unique_elements.insert(str);
                hll.add(str);
            }
            
            run_results[s].push_back(hll.count());
            true_counts[s] = (double)unique_elements.size();
        }
    }

    for (int s = 1; s <= steps; ++s) {
        double sum = 0;
        for (double val : run_results[s]) sum += val;
        double mean = sum / runs;

        double sq_sum = 0;
        for (double val : run_results[s]) sq_sum += (val - mean) * (val - mean);
        double std_dev = std::sqrt(sq_sum / runs);

        file << s << "," << (s * step_size) << "," << true_counts[s] << "," 
             << mean << "," << std_dev << "," << mem_usage << "\n";
    }
    file.close();
}

void run_experiment_imp(int b, int total_elements, int steps, int runs, const std::string& filename) {
    std::ofstream file(filename);
    file << "step,processed,true_cardinality,est_mean,est_stddev,memory_bytes\n";

    int step_size = total_elements / steps;
    
    std::vector<std::vector<double>> run_results(steps + 1);
    std::vector<double> true_counts(steps + 1);
    size_t mem_usage = 0;

    for (int r = 0; r < runs; ++r) {
        RandomStreamGen gen(42 + r);
        HyperLogLogImp hll(b);
        std::set<std::string> unique_elements;
        
        mem_usage = hll.memory_usage();

        for (int s = 0; s <= steps; ++s) {
            int target_size = s * step_size;
            
            int needed = target_size - (s == 0 ? 0 : (s-1)*step_size);
            if (s == 0) needed = 0;

            for(int k=0; k<needed; ++k) {
                std::string str = gen.next_string();
                unique_elements.insert(str);
                hll.add(str);
            }
            
            run_results[s].push_back(hll.count());
            true_counts[s] = (double)unique_elements.size();
        }
    }

    for (int s = 1; s <= steps; ++s) {
        double sum = 0;
        for (double val : run_results[s]) sum += val;
        double mean = sum / runs;

        double sq_sum = 0;
        for (double val : run_results[s]) sq_sum += (val - mean) * (val - mean);
        double std_dev = std::sqrt(sq_sum / runs);

        file << s << "," << (s * step_size) << "," << true_counts[s] << "," 
             << mean << "," << std_dev << "," << mem_usage << "\n";
    }
    file.close();
}

int main() {
    int b = 14; 
    int total_stream_size = 200000;
    int steps = 20;
    int runs = 10; 

    std::cout << "Running Standard HLL Experiment..." << std::endl;
    run_experiment_std(b, total_stream_size, steps, runs, "results_std.csv");

    std::cout << "Running Improved HLL Experiment..." << std::endl;
    run_experiment_imp(b, total_stream_size, steps, runs, "results_imp.csv");

    std::cout << "Done. Results saved to CSV." << std::endl;
    return 0;
}