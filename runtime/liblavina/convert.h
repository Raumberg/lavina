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

// Overloads for smaller integer types and size_t
inline std::string operator+(const std::string& s, int32_t n) { return s + std::to_string(n); }
inline std::string operator+(int32_t n, const std::string& s) { return std::to_string(n) + s; }
inline std::string operator+(const std::string& s, int16_t n) { return s + std::to_string(n); }
inline std::string operator+(int16_t n, const std::string& s) { return std::to_string(n) + s; }
inline std::string operator+(const std::string& s, int8_t n) { return s + std::to_string(static_cast<int>(n)); }
inline std::string operator+(int8_t n, const std::string& s) { return std::to_string(static_cast<int>(n)) + s; }
inline std::string operator+(const std::string& s, size_t n) { return s + std::to_string(n); }
inline std::string operator+(size_t n, const std::string& s) { return std::to_string(n) + s; }
inline std::string operator+(const std::string& s, float f) { return s + std::to_string(f); }
inline std::string operator+(float f, const std::string& s) { return std::to_string(f) + s; }
inline std::string operator+(const std::string& s, const char* c) { return s + std::string(c); }
inline std::string operator+(const char* c, const std::string& s) { return std::string(c) + s; }
inline std::string to_string(int32_t n) { return std::to_string(n); }
inline std::string to_string(size_t n) { return std::to_string(n); }
inline std::string to_string(float f) { return std::to_string(f); }
inline std::string to_string(const char* s) { return std::string(s); }
