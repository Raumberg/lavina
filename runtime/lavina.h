#pragma once
// ═══════════════════════════════════════════════════════════════
//  Lavina C++ Runtime Library
//  Auto-included by the Lavina compiler's C++ backend.
// ═══════════════════════════════════════════════════════════════

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include <cstdint>
#include <functional>
#include <cmath>
#include <stdexcept>
#include <variant>
#include <any>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cstdlib>

// ── Print ───────────────────────────────────────────────────────

void print(const std::string& s) { std::cout << s << std::endl; }
void print(int64_t n) { std::cout << n << std::endl; }
void print(double n) { std::cout << n << std::endl; }
void print(bool b) { std::cout << (b ? "true" : "false") << std::endl; }

template<typename T>
void print(const std::vector<T>& v) {
    std::cout << "[";
    for (size_t i = 0; i < v.size(); i++) {
        if (i > 0) std::cout << ", ";
        std::cout << v[i];
    }
    std::cout << "]" << std::endl;
}

// ── Type conversions ────────────────────────────────────────────

std::string to_string(int64_t n) { return std::to_string(n); }
std::string to_string(double n) { return std::to_string(n); }
std::string to_string(bool b) { return b ? "true" : "false"; }
std::string to_string(const std::string& s) { return s; }

// ── String + non-string concatenation ───────────────────────────

std::string operator+(const std::string& s, int64_t n) { return s + std::to_string(n); }
std::string operator+(int64_t n, const std::string& s) { return std::to_string(n) + s; }
std::string operator+(const std::string& s, double n) { return s + std::to_string(n); }
std::string operator+(double n, const std::string& s) { return std::to_string(n) + s; }
std::string operator+(const std::string& s, bool b) { return s + (b ? "true" : "false"); }
std::string operator+(bool b, const std::string& s) { return (b ? "true" : "false") + s; }

// ── String methods ──────────────────────────────────────────────

std::string lv_upper(std::string s) {
    for (auto& c : s) c = toupper(c);
    return s;
}

std::string lv_lower(std::string s) {
    for (auto& c : s) c = tolower(c);
    return s;
}

std::string lv_trim(const std::string& s) {
    auto start = s.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    auto end = s.find_last_not_of(" \t\n\r");
    return s.substr(start, end - start + 1);
}

std::string lv_replace(const std::string& s, const std::string& from, const std::string& to) {
    std::string result = s;
    size_t pos = 0;
    while ((pos = result.find(from, pos)) != std::string::npos) {
        result.replace(pos, from.length(), to);
        pos += to.length();
    }
    return result;
}

std::vector<std::string> lv_split(const std::string& s, const std::string& delim) {
    std::vector<std::string> result;
    size_t start = 0, end;
    while ((end = s.find(delim, start)) != std::string::npos) {
        result.push_back(s.substr(start, end - start));
        start = end + delim.length();
    }
    result.push_back(s.substr(start));
    return result;
}

int64_t lv_index_of(const std::string& s, const std::string& sub) {
    auto pos = s.find(sub);
    return pos == std::string::npos ? -1 : static_cast<int64_t>(pos);
}

bool lv_contains(const std::string& s, const std::string& sub) {
    return s.find(sub) != std::string::npos;
}

// ── Vector methods ──────────────────────────────────────────────

template<typename T>
T lv_pop(std::vector<T>& v) {
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

// ── HashMap methods ─────────────────────────────────────────────

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

// ── Global utility functions ────────────────────────────────────

int64_t len(const std::string& s) { return static_cast<int64_t>(s.size()); }

template<typename T>
int64_t len(const std::vector<T>& v) { return static_cast<int64_t>(v.size()); }

// ── File I/O ────────────────────────────────────────────────────

std::string fs_read(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) throw std::runtime_error("Cannot open file: " + path);
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

void fs_write(const std::string& path, const std::string& content) {
    std::ofstream f(path);
    if (!f.is_open()) throw std::runtime_error("Cannot write file: " + path);
    f << content;
}

bool fs_exists(const std::string& path) {
    std::ifstream f(path);
    return f.good();
}

// ── OS / Process ────────────────────────────────────────────────

// Populated by main() before user code runs
static std::vector<std::string> _lv_args;

std::vector<std::string> os_args() { return _lv_args; }

int64_t os_exec(const std::string& cmd) {
    return static_cast<int64_t>(std::system(cmd.c_str()));
}

std::string os_env(const std::string& name) {
    const char* val = std::getenv(name.c_str());
    return val ? std::string(val) : "";
}

[[noreturn]] void lv_exit(int64_t code) {
    std::exit(static_cast<int>(code));
}

// ── Input ───────────────────────────────────────────────────────

std::string input(const std::string& prompt = "") {
    if (!prompt.empty()) std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return line;
}
