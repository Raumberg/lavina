#pragma once
#include "core.h"
#include <random>

inline int64_t lv_abs(int64_t n) { return n < 0 ? -n : n; }
inline double lv_fabs(double n) { return std::fabs(n); }

inline int64_t lv_min(int64_t a, int64_t b) { return a < b ? a : b; }
inline int64_t lv_max(int64_t a, int64_t b) { return a > b ? a : b; }
inline double lv_fmin(double a, double b) { return a < b ? a : b; }
inline double lv_fmax(double a, double b) { return a > b ? a : b; }

inline int64_t lv_clamp(int64_t val, int64_t lo, int64_t hi) {
    return val < lo ? lo : (val > hi ? hi : val);
}
inline double lv_fclamp(double val, double lo, double hi) {
    return val < lo ? lo : (val > hi ? hi : val);
}

inline double lv_floor(double n) { return std::floor(n); }
inline double lv_ceil(double n) { return std::ceil(n); }
inline double lv_round(double n) { return std::round(n); }
inline double lv_sqrt(double n) { return std::sqrt(n); }
inline double lv_pow(double base, double exp) { return std::pow(base, exp); }
inline double lv_log(double n) { return std::log(n); }
inline double lv_log2(double n) { return std::log2(n); }
inline double lv_sin(double n) { return std::sin(n); }
inline double lv_cos(double n) { return std::cos(n); }

// Random number generation
inline int64_t lv_random(int64_t min, int64_t max) {
    static std::mt19937_64 rng(std::random_device{}());
    std::uniform_int_distribution<int64_t> dist(min, max);
    return dist(rng);
}

inline double lv_random_float() {
    static std::mt19937_64 rng(std::random_device{}());
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(rng);
}
