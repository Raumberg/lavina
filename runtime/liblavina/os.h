#pragma once
#include "core.h"

inline std::vector<std::string> _lv_args;

inline std::vector<std::string> __os_args() { return _lv_args; }

inline int64_t __os_exec(const std::string& cmd) {
    int status = std::system(cmd.c_str());
#if defined(__unix__) || defined(__APPLE__)
    if (WIFEXITED(status)) return static_cast<int64_t>(WEXITSTATUS(status));
    return static_cast<int64_t>(status);
#else
    return static_cast<int64_t>(status);
#endif
}

inline std::string __os_env(const std::string& name) {
    const char* val = std::getenv(name.c_str());
    return val ? std::string(val) : "";
}

[[noreturn]] inline void __lv_exit(int64_t code) {
    std::exit(static_cast<int>(code));
}

// Millisecond clock (monotonic)
inline int64_t __os_clock() {
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}

// Sleep for N milliseconds
inline void __os_sleep(int64_t ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

// Current working directory
inline std::string __os_cwd() {
    return std::filesystem::current_path().string();
}

// Bootstrap compat aliases
inline auto os_args = __os_args;
inline auto os_exec = __os_exec;
inline auto os_env = __os_env;
inline auto& lv_exit = __lv_exit;
inline auto os_clock = __os_clock;
inline auto os_sleep = __os_sleep;
inline auto os_cwd = __os_cwd;
