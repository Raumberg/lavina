#pragma once
#include "core.h"

inline std::string fs_read(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) throw std::runtime_error("Cannot open file: " + path);
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

inline void fs_write(const std::string& path, const std::string& content) {
    std::ofstream f(path);
    if (!f.is_open()) throw std::runtime_error("Cannot write file: " + path);
    f << content;
}

inline bool fs_exists(const std::string& path) {
    std::ifstream f(path);
    return f.good();
}

inline std::string input(const std::string& prompt = "") {
    if (!prompt.empty()) std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return line;
}
