#pragma once
#include <chrono>

inline double now_seconds() {
    using namespace std::chrono;
    return duration_cast<duration<double>>(high_resolution_clock::now().time_since_epoch()).count();
}
