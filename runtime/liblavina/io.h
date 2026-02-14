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

inline void fs_append(const std::string& path, const std::string& content) {
    std::ofstream f(path, std::ios::app);
    if (!f.is_open()) throw std::runtime_error("Cannot append to file: " + path);
    f << content;
}

inline std::vector<std::string> fs_read_lines(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) throw std::runtime_error("Cannot open file: " + path);
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(f, line)) lines.push_back(line);
    return lines;
}

inline bool fs_remove(const std::string& path) {
    return std::remove(path.c_str()) == 0;
}

inline bool fs_is_dir(const std::string& path) {
    return std::filesystem::is_directory(path);
}

inline std::vector<std::string> fs_listdir(const std::string& path) {
    std::vector<std::string> entries;
    for (const auto& e : std::filesystem::directory_iterator(path)) {
        entries.push_back(e.path().filename().string());
    }
    std::sort(entries.begin(), entries.end());
    return entries;
}
