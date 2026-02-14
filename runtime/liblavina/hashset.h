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
