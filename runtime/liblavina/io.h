#pragma once
#include "core.h"

inline std::string __fs_read(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) throw std::runtime_error("Cannot open file: " + path);
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

inline void __fs_write(const std::string& path, const std::string& content) {
    std::ofstream f(path);
    if (!f.is_open()) throw std::runtime_error("Cannot write file: " + path);
    f << content;
}

inline bool __fs_exists(const std::string& path) {
    std::ifstream f(path);
    return f.good();
}

inline std::string input(const std::string& prompt = "") {
    if (!prompt.empty()) std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return line;
}

inline void __fs_append(const std::string& path, const std::string& content) {
    std::ofstream f(path, std::ios::app);
    if (!f.is_open()) throw std::runtime_error("Cannot append to file: " + path);
    f << content;
}

inline std::vector<std::string> __fs_read_lines(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) throw std::runtime_error("Cannot open file: " + path);
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(f, line)) lines.push_back(line);
    return lines;
}

inline bool __fs_remove(const std::string& path) {
    return std::remove(path.c_str()) == 0;
}

inline bool __fs_is_dir(const std::string& path) {
    return std::filesystem::is_directory(path);
}

inline std::vector<std::string> __fs_listdir(const std::string& path) {
    std::vector<std::string> entries;
    for (const auto& e : std::filesystem::directory_iterator(path)) {
        entries.push_back(e.path().filename().string());
    }
    std::sort(entries.begin(), entries.end());
    return entries;
}

inline bool __fs_mkdir(const std::string& path) {
    return std::filesystem::create_directories(path);
}

inline bool __fs_copy(const std::string& src, const std::string& dst) {
    std::filesystem::copy_file(src, dst, std::filesystem::copy_options::overwrite_existing);
    return true;
}

inline void __fs_rename(const std::string& from, const std::string& to) {
    std::filesystem::rename(from, to);
}

inline std::string __fs_absolute(const std::string& path) {
    return std::filesystem::absolute(path).string();
}

inline std::string __fs_basename(const std::string& path) {
    return std::filesystem::path(path).filename().string();
}

inline std::string __fs_dirname(const std::string& path) {
    return std::filesystem::path(path).parent_path().string();
}

inline int64_t __fs_size(const std::string& path) {
    return static_cast<int64_t>(std::filesystem::file_size(path));
}

// Legacy aliases (used by test_stdlib.lv; prefer __-prefixed or std::fs module)
inline auto fs_read = __fs_read;
inline auto fs_write = __fs_write;
inline auto fs_exists = __fs_exists;
inline auto fs_append = __fs_append;
inline auto fs_read_lines = __fs_read_lines;
inline auto fs_remove = __fs_remove;
inline auto fs_is_dir = __fs_is_dir;
inline auto fs_listdir = __fs_listdir;
inline auto fs_mkdir = __fs_mkdir;
inline auto fs_copy = __fs_copy;
inline auto fs_rename = __fs_rename;
inline auto fs_absolute = __fs_absolute;
inline auto fs_basename = __fs_basename;
inline auto fs_dirname = __fs_dirname;
inline auto fs_size = __fs_size;
