#pragma once
#include "core.h"

inline void lv_assert(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "ASSERTION FAILED: " << message << std::endl;
        std::exit(1);
    }
}

inline int64_t len(const std::string& s) { return static_cast<int64_t>(s.size()); }

template<typename T>
int64_t len(const std::vector<T>& v) { return static_cast<int64_t>(v.size()); }

template<typename K, typename V>
int64_t len(const std::unordered_map<K, V>& m) { return static_cast<int64_t>(m.size()); }

template<typename T>
int64_t len(const std::unordered_set<T>& s) { return static_cast<int64_t>(s.size()); }
