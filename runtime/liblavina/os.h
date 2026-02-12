#pragma once
#include "core.h"

static std::vector<std::string> _lv_args;

std::vector<std::string> os_args() { return _lv_args; }

int64_t os_exec(const std::string& cmd) {
    int status = std::system(cmd.c_str());
#if defined(__unix__) || defined(__APPLE__)
    if (WIFEXITED(status)) return static_cast<int64_t>(WEXITSTATUS(status));
    return static_cast<int64_t>(status);
#else
    return static_cast<int64_t>(status);
#endif
}

std::string os_env(const std::string& name) {
    const char* val = std::getenv(name.c_str());
    return val ? std::string(val) : "";
}

[[noreturn]] void lv_exit(int64_t code) {
    std::exit(static_cast<int>(code));
}
