#pragma once
#include "core.h"

template<typename K, typename V>
std::vector<K> lv_keys(const std::unordered_map<K, V>& m) {
    std::vector<K> result;
    for (auto& [k, v] : m) result.push_back(k);
    return result;
}

template<typename K, typename V>
std::vector<V> lv_values(const std::unordered_map<K, V>& m) {
    std::vector<V> result;
    for (auto& [k, v] : m) result.push_back(v);
    return result;
}

template<typename K, typename V>
void lv_remove(std::unordered_map<K, V>& m, const K& key) {
    m.erase(key);
}
