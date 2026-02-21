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

// --- collections module functions ---

// Range [start, end)
inline std::vector<int64_t> __lv_col_range(int64_t start, int64_t end) {
    std::vector<int64_t> result;
    for (int64_t i = start; i < end; i++) result.push_back(i);
    return result;
}

// Range with step
inline std::vector<int64_t> __lv_col_range_step(int64_t start, int64_t end, int64_t step) {
    std::vector<int64_t> result;
    if (step > 0) {
        for (int64_t i = start; i < end; i += step) result.push_back(i);
    } else if (step < 0) {
        for (int64_t i = start; i > end; i += step) result.push_back(i);
    }
    return result;
}

// Take first n elements
template<typename T>
std::vector<T> __lv_col_take(const std::vector<T>& v, int64_t n) {
    if (n <= 0) return {};
    if (n >= static_cast<int64_t>(v.size())) return v;
    return std::vector<T>(v.begin(), v.begin() + n);
}

// Drop first n elements
template<typename T>
std::vector<T> __lv_col_drop(const std::vector<T>& v, int64_t n) {
    if (n <= 0) return v;
    if (n >= static_cast<int64_t>(v.size())) return {};
    return std::vector<T>(v.begin() + n, v.end());
}

// Map: apply f to each element, return new vector
template<typename T, typename F>
auto __lv_col_map(const std::vector<T>& v, F f) {
    using R = decltype(f(v[0]));
    std::vector<R> result;
    result.reserve(v.size());
    for (const auto& x : v) result.push_back(f(x));
    return result;
}

// Filter: return elements where f(x) is true
template<typename T, typename F>
std::vector<T> __lv_col_filter(const std::vector<T>& v, F f) {
    std::vector<T> result;
    for (const auto& x : v) {
        if (f(x)) result.push_back(x);
    }
    return result;
}

// Reduce: fold left with initial value
template<typename T, typename F, typename A>
A __lv_col_reduce(const std::vector<T>& v, F f, A init) {
    A acc = init;
    for (const auto& x : v) acc = f(acc, x);
    return acc;
}

// For each: apply f to each element (void)
template<typename T, typename F>
void __lv_col_for_each(const std::vector<T>& v, F f) {
    for (const auto& x : v) f(x);
}

// Zip: combine two vectors into vector of pairs
template<typename A, typename B>
std::vector<std::pair<A, B>> __lv_col_zip(const std::vector<A>& a, const std::vector<B>& b) {
    std::vector<std::pair<A, B>> result;
    size_t len = std::min(a.size(), b.size());
    result.reserve(len);
    for (size_t i = 0; i < len; i++) result.push_back({a[i], b[i]});
    return result;
}

// Enumerate: pair each element with its index
template<typename T>
std::vector<std::pair<int64_t, T>> __lv_col_enumerate(const std::vector<T>& v) {
    std::vector<std::pair<int64_t, T>> result;
    result.reserve(v.size());
    for (size_t i = 0; i < v.size(); i++) {
        result.push_back({static_cast<int64_t>(i), v[i]});
    }
    return result;
}
