#pragma once
#include "core.h"
#include <random>

inline int64_t __lv_abs(int64_t n) { return n < 0 ? -n : n; }
inline double __lv_fabs(double n) { return std::fabs(n); }

inline int64_t __lv_min(int64_t a, int64_t b) { return a < b ? a : b; }
inline int64_t __lv_max(int64_t a, int64_t b) { return a > b ? a : b; }
inline double __lv_fmin(double a, double b) { return a < b ? a : b; }
inline double __lv_fmax(double a, double b) { return a > b ? a : b; }

inline int64_t __lv_clamp(int64_t val, int64_t lo, int64_t hi) {
    return val < lo ? lo : (val > hi ? hi : val);
}
inline double __lv_fclamp(double val, double lo, double hi) {
    return val < lo ? lo : (val > hi ? hi : val);
}

inline double __lv_floor(double n) { return std::floor(n); }
inline double __lv_ceil(double n) { return std::ceil(n); }
inline double __lv_round(double n) { return std::round(n); }
inline double __lv_sqrt(double n) { return std::sqrt(n); }
inline double __lv_pow(double base, double exp) { return std::pow(base, exp); }
inline double __lv_log(double n) { return std::log(n); }
inline double __lv_log2(double n) { return std::log2(n); }
inline double __lv_sin(double n) { return std::sin(n); }
inline double __lv_cos(double n) { return std::cos(n); }
inline double __lv_tan(double n) { return std::tan(n); }
inline double __lv_asin(double n) { return std::asin(n); }
inline double __lv_acos(double n) { return std::acos(n); }
inline double __lv_atan(double n) { return std::atan(n); }
inline double __lv_atan2(double y, double x) { return std::atan2(y, x); }
inline double __lv_exp(double n) { return std::exp(n); }
inline double __lv_log10(double n) { return std::log10(n); }
inline double __lv_pi() { return M_PI; }
inline double __lv_e() { return M_E; }

// Random number generation
inline int64_t __lv_random(int64_t min, int64_t max) {
    static std::mt19937_64 rng(std::random_device{}());
    std::uniform_int_distribution<int64_t> dist(min, max);
    return dist(rng);
}

inline double __lv_random_float() {
    static std::mt19937_64 rng(std::random_device{}());
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(rng);
}

// Bootstrap compat aliases
inline auto lv_abs = __lv_abs;
inline auto lv_fabs = __lv_fabs;
inline auto lv_min = __lv_min;
inline auto lv_max = __lv_max;
inline auto lv_fmin = __lv_fmin;
inline auto lv_fmax = __lv_fmax;
inline auto lv_clamp = __lv_clamp;
inline auto lv_fclamp = __lv_fclamp;
inline auto lv_floor = __lv_floor;
inline auto lv_ceil = __lv_ceil;
inline auto lv_round = __lv_round;
inline auto lv_sqrt = __lv_sqrt;
inline auto lv_pow = __lv_pow;
inline auto lv_log = __lv_log;
inline auto lv_log2 = __lv_log2;
inline auto lv_sin = __lv_sin;
inline auto lv_cos = __lv_cos;
inline auto lv_random = __lv_random;
inline auto lv_random_float = __lv_random_float;
