#pragma once
#include "core.h"

template<typename T>
bool lv_contains(const std::unordered_set<T>& s, const T& val) {
    return s.count(val) > 0;
}

template<typename T>
void lv_remove(std::unordered_set<T>& s, const T& val) {
    s.erase(val);
}

// --- collections module functions ---

// Set union
template<typename T>
std::unordered_set<T> __lv_col_set_union(const std::unordered_set<T>& a, const std::unordered_set<T>& b) {
    std::unordered_set<T> result = a;
    for (const auto& x : b) result.insert(x);
    return result;
}

// Set intersection
template<typename T>
std::unordered_set<T> __lv_col_set_intersection(const std::unordered_set<T>& a, const std::unordered_set<T>& b) {
    std::unordered_set<T> result;
    for (const auto& x : a) {
        if (b.count(x) > 0) result.insert(x);
    }
    return result;
}

// Set difference (a - b)
template<typename T>
std::unordered_set<T> __lv_col_set_difference(const std::unordered_set<T>& a, const std::unordered_set<T>& b) {
    std::unordered_set<T> result;
    for (const auto& x : a) {
        if (b.count(x) == 0) result.insert(x);
    }
    return result;
}
