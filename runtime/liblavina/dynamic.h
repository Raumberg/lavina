#pragma once
#include "core.h"

// std::any comparison operators
inline bool operator==(const std::any& a, const std::string& b) {
    if (auto* s = std::any_cast<std::string>(&a)) return *s == b;
    return false;
}
inline bool operator!=(const std::any& a, const std::string& b) { return !(a == b); }
inline bool operator==(const std::string& a, const std::any& b) { return b == a; }
inline bool operator!=(const std::string& a, const std::any& b) { return !(b == a); }

inline bool operator==(const std::any& a, const std::any& b) {
    if (auto* sa = std::any_cast<std::string>(&a)) {
        if (auto* sb = std::any_cast<std::string>(&b)) return *sa == *sb;
    }
    if (auto* ia = std::any_cast<int64_t>(&a)) {
        if (auto* ib = std::any_cast<int64_t>(&b)) return *ia == *ib;
    }
    if (auto* ba = std::any_cast<bool>(&a)) {
        if (auto* bb = std::any_cast<bool>(&b)) return *ba == *bb;
    }
    if (!a.has_value() && !b.has_value()) return true;
    return false;
}
inline bool operator!=(const std::any& a, const std::any& b) { return !(a == b); }

inline bool operator==(const std::any& a, bool b) {
    if (auto* v = std::any_cast<bool>(&a)) return *v == b;
    return false;
}
inline bool operator!=(const std::any& a, bool b) { return !(a == b); }
inline bool operator==(bool a, const std::any& b) { return b == a; }
inline bool operator!=(bool a, const std::any& b) { return !(b == a); }

inline bool operator==(const std::any& a, int64_t b) {
    if (auto* v = std::any_cast<int64_t>(&a)) return *v == b;
    return false;
}
inline bool operator!=(const std::any& a, int64_t b) { return !(a == b); }

// std::any string concatenation
inline std::string operator+(const std::string& s, const std::any& d) {
    if (auto* v = std::any_cast<std::string>(&d)) return s + *v;
    if (auto* v = std::any_cast<int64_t>(&d)) return s + std::to_string(*v);
    if (auto* v = std::any_cast<double>(&d)) return s + std::to_string(*v);
    if (auto* v = std::any_cast<bool>(&d)) return s + (*v ? "true" : "false");
    return s + "<dynamic>";
}
inline std::string operator+(const std::any& d, const std::string& s) {
    if (auto* v = std::any_cast<std::string>(&d)) return *v + s;
    if (auto* v = std::any_cast<int64_t>(&d)) return std::to_string(*v) + s;
    if (auto* v = std::any_cast<double>(&d)) return std::to_string(*v) + s;
    if (auto* v = std::any_cast<bool>(&d)) return std::string(*v ? "true" : "false") + s;
    return "<dynamic>" + s;
}

// std::any truthiness
inline bool lv_is_truthy(const std::any& a) {
    if (!a.has_value()) return false;
    if (auto* v = std::any_cast<bool>(&a)) return *v;
    if (auto* v = std::any_cast<int64_t>(&a)) return *v != 0;
    if (auto* v = std::any_cast<std::string>(&a)) return !v->empty();
    return true;
}

// print for std::any
inline void print(const std::any& a) {
    if (auto* v = std::any_cast<std::string>(&a)) { std::cout << *v << std::endl; return; }
    if (auto* v = std::any_cast<int64_t>(&a)) { std::cout << *v << std::endl; return; }
    if (auto* v = std::any_cast<double>(&a)) { std::cout << *v << std::endl; return; }
    if (auto* v = std::any_cast<bool>(&a)) { std::cout << (*v ? "true" : "false") << std::endl; return; }
    if (!a.has_value()) { std::cout << "null" << std::endl; return; }
    std::cout << "<dynamic>" << std::endl;
}

// len for std::any
inline int64_t lv_len(const std::any& a) {
    if (auto* v = std::any_cast<std::string>(&a)) return static_cast<int64_t>(v->size());
    return 0;
}
