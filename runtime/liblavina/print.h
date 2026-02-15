#pragma once
#include "core.h"

inline void print(const std::string& s) { std::cout << s << std::endl; }
inline void print(int64_t n) { std::cout << n << std::endl; }
inline void print(double n) { std::cout << n << std::endl; }
inline void print(bool b) { std::cout << (b ? "true" : "false") << std::endl; }
inline void print(int32_t n) { std::cout << n << std::endl; }
inline void print(int16_t n) { std::cout << n << std::endl; }
inline void print(int8_t n) { std::cout << static_cast<int>(n) << std::endl; }
inline void print(float f) { std::cout << f << std::endl; }
inline void print(size_t n) { std::cout << n << std::endl; }

// println — same as print
inline void println(const std::string& s) { std::cout << s << std::endl; }
inline void println(int64_t n) { std::cout << n << std::endl; }
inline void println(double n) { std::cout << n << std::endl; }
inline void println(bool b) { std::cout << (b ? "true" : "false") << std::endl; }
inline void println(int32_t n) { std::cout << n << std::endl; }
inline void println(int16_t n) { std::cout << n << std::endl; }
inline void println(int8_t n) { std::cout << static_cast<int>(n) << std::endl; }
inline void println(float f) { std::cout << f << std::endl; }
inline void println(size_t n) { std::cout << n << std::endl; }
inline void println() { std::cout << std::endl; }

template<typename T>
void print(const std::vector<T>& v) {
    std::cout << "[";
    for (size_t i = 0; i < v.size(); i++) {
        if (i > 0) std::cout << ", ";
        std::cout << v[i];
    }
    std::cout << "]" << std::endl;
}

template<typename T>
void println(const std::vector<T>& v) { print(v); }
