#pragma once
#include "core.h"

template<typename T>
T lv_pop(std::vector<T>& v) {
    if (v.empty()) throw std::runtime_error("pop on empty vector");
    T val = v.back();
    v.pop_back();
    return val;
}

template<typename T>
void lv_remove(std::vector<T>& v, int64_t idx) {
    v.erase(v.begin() + idx);
}

template<typename T>
std::string lv_join(const std::vector<T>& v, const std::string& sep) {
    std::string result;
    for (size_t i = 0; i < v.size(); i++) {
        if (i > 0) result += sep;
        if constexpr (std::is_same_v<T, std::string>) result += v[i];
        else result += std::to_string(v[i]);
    }
    return result;
}

template<typename T>
void lv_reverse(std::vector<T>& v) {
    std::reverse(v.begin(), v.end());
}

template<typename T>
bool lv_contains(const std::vector<T>& v, const T& val) {
    return std::find(v.begin(), v.end(), val) != v.end();
}
