#pragma once
#include "core.h"

inline std::string to_string(int64_t n) { return std::to_string(n); }
inline std::string to_string(double n) { return std::to_string(n); }
inline std::string to_string(bool b) { return b ? "true" : "false"; }
inline std::string to_string(const std::string& s) { return s; }

// String + non-string concatenation
inline std::string operator+(const std::string& s, int64_t n) { return s + std::to_string(n); }
inline std::string operator+(int64_t n, const std::string& s) { return std::to_string(n) + s; }
inline std::string operator+(const std::string& s, double n) { return s + std::to_string(n); }
inline std::string operator+(double n, const std::string& s) { return std::to_string(n) + s; }
inline std::string operator+(const std::string& s, bool b) { return s + (b ? "true" : "false"); }
inline std::string operator+(bool b, const std::string& s) { return (b ? "true" : "false") + s; }
