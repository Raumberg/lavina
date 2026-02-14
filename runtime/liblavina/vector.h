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

// Slice: vector[start:end]
template<typename T>
std::vector<T> lv_slice(const std::vector<T>& v, int64_t start, int64_t end) {
    if (start < 0) start = 0;
    if (end > static_cast<int64_t>(v.size())) end = v.size();
    if (start >= end) return {};
    return std::vector<T>(v.begin() + start, v.begin() + end);
}

// Sort (ascending)
template<typename T>
void lv_sort(std::vector<T>& v) {
    std::sort(v.begin(), v.end());
}

// Sort with comparator lambda
template<typename T, typename F>
void lv_sort(std::vector<T>& v, F cmp) {
    std::sort(v.begin(), v.end(), cmp);
}

// Find index of element (-1 if not found)
template<typename T>
int64_t lv_index_of(const std::vector<T>& v, const T& val) {
    auto it = std::find(v.begin(), v.end(), val);
    return it == v.end() ? -1 : static_cast<int64_t>(it - v.begin());
}

// Deduplicate (preserves order)
template<typename T>
std::vector<T> lv_unique(const std::vector<T>& v) {
    std::vector<T> result;
    std::unordered_set<T> seen;
    for (const auto& x : v) {
        if (seen.insert(x).second) result.push_back(x);
    }
    return result;
}

// Flatten vector of vectors
template<typename T>
std::vector<T> lv_flatten(const std::vector<std::vector<T>>& v) {
    std::vector<T> result;
    for (const auto& inner : v)
        for (const auto& x : inner)
            result.push_back(x);
    return result;
}
