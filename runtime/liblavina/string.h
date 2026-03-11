#pragma once
#include "core.h"

inline std::string lv_upper(std::string s) {
    for (auto& c : s) c = toupper(c);
    return s;
}

inline std::string lv_lower(std::string s) {
    for (auto& c : s) c = tolower(c);
    return s;
}

inline std::string lv_trim(const std::string& s) {
    auto start = s.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    auto end = s.find_last_not_of(" \t\n\r");
    return s.substr(start, end - start + 1);
}

inline std::string lv_replace(const std::string& s, const std::string& from, const std::string& to) {
    std::string result = s;
    size_t pos = 0;
    while ((pos = result.find(from, pos)) != std::string::npos) {
        result.replace(pos, from.length(), to);
        pos += to.length();
    }
    return result;
}

inline int lv_compare(const std::string& a, const std::string& b) {
    size_t i = 0;
    while (i < a.length() && i < b.length() && a[i] == b[i]) {
        i++;
    }

    if (i == a.length() && i == b.length()) {
    return 0;
    }
    return 1;
}

inline std::string lv_reverse(const std::string& s) {
    return {s.rbegin(), s.rend()};
}

inline std::vector<std::string> lv_split(const std::string& s, const std::string& delim) {
    std::vector<std::string> result;
    size_t start = 0, end;
    while ((end = s.find(delim, start)) != std::string::npos) {
        result.push_back(s.substr(start, end - start));
        start = end + delim.length();
    }
    result.push_back(s.substr(start));
    return result;
}

inline int64_t lv_index_of(const std::string& s, const std::string& sub) {
    auto pos = s.find(sub);
    return pos == std::string::npos ? -1 : static_cast<int64_t>(pos);
}

inline bool lv_contains(const std::string& s, const std::string& sub) {
    return s.find(sub) != std::string::npos;
}

// index_of with start offset
inline int64_t lv_index_of(const std::string& s, const std::string& sub, int64_t from) {
    if (from < 0) from = 0;
    auto pos = s.find(sub, static_cast<size_t>(from));
    return pos == std::string::npos ? -1 : static_cast<int64_t>(pos);
}

// String ↔ number conversions
inline int64_t __str_to_int(const std::string& s) {
    return std::stoll(s);
}

inline double __str_to_float(const std::string& s) {
    return std::stod(s);
}

inline std::string __int_to_str(int64_t n) {
    return std::to_string(n);
}

inline std::string __float_to_str(double n) {
    return std::to_string(n);
}

// Repeat a string N times
inline std::string lv_repeat(const std::string& s, int64_t n) {
    std::string result;
    result.reserve(s.size() * n);
    for (int64_t i = 0; i < n; i++) result += s;
    return result;
}

// Pad left/right to target width
inline std::string lv_pad_left(const std::string& s, int64_t width, const std::string& fill = " ") {
    if (static_cast<int64_t>(s.size()) >= width) return s;
    std::string pad;
    while (static_cast<int64_t>(pad.size() + s.size()) < width) pad += fill;
    return pad + s;
}

inline std::string lv_pad_right(const std::string& s, int64_t width, const std::string& fill = " ") {
    if (static_cast<int64_t>(s.size()) >= width) return s;
    std::string result = s;
    while (static_cast<int64_t>(result.size()) < width) result += fill;
    return result;
}

// Count occurrences of substring
inline int64_t __lv_count(const std::string& s, const std::string& sub) {
    if (sub.empty()) return 0;
    int64_t count = 0;
    size_t pos = 0;
    while ((pos = s.find(sub, pos)) != std::string::npos) {
        count++;
        pos += sub.size();
    }
    return count;
}

// Legacy aliases (used by test_stdlib.lv; prefer __-prefixed or std:: modules)
inline auto str_to_int = __str_to_int;
inline auto str_to_float = __str_to_float;
inline auto int_to_str = __int_to_str;
inline auto float_to_str = __float_to_str;
inline auto lv_count = __lv_count;
