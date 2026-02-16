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
#include <sys/wait.h>
#include <unistd.h>
#include <climits>
#include <openssl/sha.h>
#include <openssl/md5.h>
#endif
#if defined(__APPLE__)
#include <mach-o/dyld.h>
#endif
#if defined(_WIN32)
#include <windows.h>
#include <wincrypt.h>
#endif