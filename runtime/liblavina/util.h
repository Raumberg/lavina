#pragma once
#include "core.h"

int64_t len(const std::string& s) { return static_cast<int64_t>(s.size()); }

template<typename T>
int64_t len(const std::vector<T>& v) { return static_cast<int64_t>(v.size()); }
