#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <optional>
#include <cstdint>
#include <cinttypes>
#include <functional>
#include <cmath>
#include <stdexcept>
#include <variant>
#include <any>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <filesystem>
#include <array>
#include <sstream>
#include <iomanip>
#include <ctime>
#if defined(__unix__) || defined(__APPLE__)
#define RAND_PLATFORM_UNIX
#include <sys/wait.h>
#include <unistd.h>
#include <climits>
#endif
#if defined(__APPLE__)
#define RAND_PLATFORM_MACOS
#include <mach-o/dyld.h>
#endif
#if defined(_WIN32)
#define RAND_PLATFORM_WINDOWS
#include <windows.h>
#include <bcrypt.h>
#include <wincrypt.h>
#endif