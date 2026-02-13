#pragma once
#include "core.h"

inline void print(const std::string& s) { std::cout << s << std::endl; }
inline void print(int64_t n) { std::cout << n << std::endl; }
inline void print(double n) { std::cout << n << std::endl; }
inline void print(bool b) { std::cout << (b ? "true" : "false") << std::endl; }

template<typename T>
void print(const std::vector<T>& v) {
    std::cout << "[";
    for (size_t i = 0; i < v.size(); i++) {
        if (i > 0) std::cout << ", ";
        std::cout << v[i];
    }
    std::cout << "]" << std::endl;
}
