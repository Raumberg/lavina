#pragma once
#include "core.h"

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
