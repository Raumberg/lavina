#pragma once
#include "core.h"

namespace TimeUtils {
    struct DateTime {
        int year;
        int month;        // 1-12
        int day;          // 1-31
        int hour;         // 0-23
        int minute;       // 0-59
        int second;       // 0-59
        int millisecond;  // 0-999
        int microsecond;  // 0-999
        int dayOfWeek;    // 0-6 (Sunday = 0)
        int dayOfYear;    // 1-366
        
        DateTime() : year(0), month(0), day(0), hour(0), minute(0), second(0),
                    millisecond(0), microsecond(0), dayOfWeek(0), dayOfYear(0) {}
    };

    struct Duration {
        long long milliseconds;
        
        Duration() : milliseconds(0) {}
        explicit Duration(long long ms) : milliseconds(ms) {}
        
        long long days() const { return milliseconds / (1000LL * 60 * 60 * 24); }
        long long hours() const { return milliseconds / (1000LL * 60 * 60); }
        long long minutes() const { return milliseconds / (1000LL * 60); }
        long long seconds() const { return milliseconds / 1000LL; }
        
        double totalDays() const { return milliseconds / (1000.0 * 60 * 60 * 24); }
        double totalHours() const { return milliseconds / (1000.0 * 60 * 60); }
        double totalMinutes() const { return milliseconds / (1000.0 * 60); }
        double totalSeconds() const { return milliseconds / 1000.0; }
    };

    // High-resolution timer

    class Timer {
    private:
        std::chrono::high_resolution_clock::time_point start_time;
        bool running;
        long long elapsed_ns;
        
    public:
        Timer() : running(false), elapsed_ns(0) {}
        
        void start() {
            start_time = std::chrono::high_resolution_clock::now();
            running = true;
            elapsed_ns = 0;
        }
        
        void stop() {
            if (running) {
                auto end_time = std::chrono::high_resolution_clock::now();
                elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    end_time - start_time).count();
                running = false;
            }
        }
        
        void reset() {
            start_time = std::chrono::high_resolution_clock::now();
            elapsed_ns = 0;
            running = false;
        }
        
        long long elapsedNanoseconds() const {
            if (running) {
                auto current = std::chrono::high_resolution_clock::now();
                return std::chrono::duration_cast<std::chrono::nanoseconds>(
                    current - start_time).count();
            }
            return elapsed_ns;
        }
        
        long long elapsedMicroseconds() const { return elapsedNanoseconds() / 1000; }
        long long elapsedMilliseconds() const { return elapsedNanoseconds() / 1000000; }
        double elapsedSeconds() const { return elapsedNanoseconds() / 1000000000.0; }
        
        bool isRunning() const { return running; }
    };

    // System time functions

    // Get current Unix timestamp in seconds
    inline long long getCurrentTimestamp() {
        return std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();
    }

    // Get current timestamp in milliseconds
    inline long long getCurrentTimestampMs() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();
    }

    // Get current timestamp in microseconds
    inline long long getCurrentTimestampUs() {
        return std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();
    }

    // Get system uptime in milliseconds
    inline long long getSystemUptime() {
    #ifdef _WIN32
        return GetTickCount64();
    #else
        struct timespec ts;
        if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0) {
            return static_cast<long long>(ts.tv_sec) * 1000LL + ts.tv_nsec / 1000000LL;
        }
        return 0;
    #endif
    }

    // Get high-resolution performance counter
    inline long long getPerformanceCounter() {
    #ifdef _WIN32
        LARGE_INTEGER counter;
        QueryPerformanceCounter(&counter);
        return counter.QuadPart;
    #else
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return static_cast<long long>(ts.tv_sec) * 1000000000LL + ts.tv_nsec;
    #endif
    }

    // Get performance counter frequency (ticks per second)
    inline long long getPerformanceFrequency() {
    #ifdef _WIN32
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        return frequency.QuadPart;
    #else
        return 1000000000LL; // nanoseconds
    #endif
    }

    // DateTime conversion functions

    // Convert Unix timestamp to DateTime structure
    inline DateTime timestampToDateTime(long long timestamp, bool localTime = true) {
        DateTime dt;
        time_t time = static_cast<time_t>(timestamp);
        
    #ifdef _WIN32
        struct tm timeinfo;
        if (localTime) {
            localtime_s(&timeinfo, &time);
        } else {
            gmtime_s(&timeinfo, &time);
        }
    #else
        struct tm timeinfo;
        if (localTime) {
            localtime_r(&time, &timeinfo);
        } else {
            gmtime_r(&time, &timeinfo);
        }
    #endif
        
        dt.year = timeinfo.tm_year + 1900;
        dt.month = timeinfo.tm_mon + 1;
        dt.day = timeinfo.tm_mday;
        dt.hour = timeinfo.tm_hour;
        dt.minute = timeinfo.tm_min;
        dt.second = timeinfo.tm_sec;
        dt.dayOfWeek = timeinfo.tm_wday;
        dt.dayOfYear = timeinfo.tm_yday + 1;
        
        // Add milliseconds/microseconds if available
        long long ms = timestamp % 1000;
        if (ms < 0) ms += 1000;
        dt.millisecond = static_cast<int>(ms);
        dt.microsecond = 0;
        
        return dt;
    }

    // Get current DateTime
    inline DateTime getCurrentDateTime(bool localTime = true) {
        return timestampToDateTime(getCurrentTimestamp(), localTime);
    }

    // Convert DateTime to Unix timestamp
    inline long long dateTimeToTimestamp(const DateTime& dt) {
        struct tm timeinfo = {};
        timeinfo.tm_year = dt.year - 1900;
        timeinfo.tm_mon = dt.month - 1;
        timeinfo.tm_mday = dt.day;
        timeinfo.tm_hour = dt.hour;
        timeinfo.tm_min = dt.minute;
        timeinfo.tm_sec = dt.second;
        timeinfo.tm_isdst = -1;
        
        return static_cast<long long>(mktime(&timeinfo));
    }

    // Formatting functions

    // Format DateTime as string
    inline std::string formatDateTime(const DateTime& dt, const std::string& format = "%Y-%m-%d %H:%M:%S") {
        struct tm timeinfo = {};
        timeinfo.tm_year = dt.year - 1900;
        timeinfo.tm_mon = dt.month - 1;
        timeinfo.tm_mday = dt.day;
        timeinfo.tm_hour = dt.hour;
        timeinfo.tm_min = dt.minute;
        timeinfo.tm_sec = dt.second;
        timeinfo.tm_wday = dt.dayOfWeek;
        timeinfo.tm_yday = dt.dayOfYear - 1;
        
        char buffer[256];
        strftime(buffer, sizeof(buffer), format.c_str(), &timeinfo);
        
        std::string result(buffer);
        
        // Handle milliseconds placeholder {ms}
        size_t ms_pos = result.find("{ms}");
        if (ms_pos != std::string::npos) {
            std::ostringstream oss;
            oss << std::setw(3) << std::setfill('0') << dt.millisecond;
            result.replace(ms_pos, 4, oss.str());
        }
        
        return result;
    }

    // Format current time as string
    inline std::string formatCurrentTime(const std::string& format = "%Y-%m-%d %H:%M:%S", bool localTime = true) {
        return formatDateTime(getCurrentDateTime(localTime), format);
    }

    // Format timestamp as ISO 8601 string
    inline std::string toISO8601(long long timestamp, bool localTime = true) {
        DateTime dt = timestampToDateTime(timestamp, localTime);
        std::ostringstream oss;
        oss << std::setfill('0')
            << std::setw(4) << dt.year << "-"
            << std::setw(2) << dt.month << "-"
            << std::setw(2) << dt.day << "T"
            << std::setw(2) << dt.hour << ":"
            << std::setw(2) << dt.minute << ":"
            << std::setw(2) << dt.second;
        
        if (!localTime) {
            oss << "Z";
        }
        
        return oss.str();
    }

    // Parse ISO 8601 string to timestamp (basic)
    inline long long fromISO8601(const std::string& iso) {
        DateTime dt;
        char delimiter;
        std::istringstream iss(iso);
        
        iss >> dt.year >> delimiter >> dt.month >> delimiter >> dt.day
            >> delimiter >> dt.hour >> delimiter >> dt.minute >> delimiter >> dt.second;
        
        if (iss.fail()) {
            return 0;
        }
        
        return dateTimeToTimestamp(dt);
    }

    // Sleep functions

    // Sleep for specified milliseconds
    inline void sleepMs(long long milliseconds) {
    #ifdef _WIN32
        Sleep(static_cast<DWORD>(milliseconds));
    #else
        usleep(static_cast<useconds_t>(milliseconds * 1000));
    #endif
    }

    // Sleep for specified microseconds
    inline void sleepUs(long long microseconds) {
    #ifdef _WIN32
        // Windows Sleep has millisecond precision, so we use busy-wait for sub-millisecond
        if (microseconds < 1000) {
            auto start = std::chrono::high_resolution_clock::now();
            while (true) {
                auto now = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - start).count();
                if (elapsed >= microseconds) break;
            }
        } else {
            Sleep(static_cast<DWORD>(microseconds / 1000));
        }
    #else
        usleep(static_cast<useconds_t>(microseconds));
    #endif
    }

    // Sleep for specified seconds
    inline void sleepSeconds(double seconds) {
        sleepMs(static_cast<long long>(seconds * 1000));
    }

    // Utility functions

    // Calculate difference between two timestamps in milliseconds
    inline long long timeDifference(long long timestamp1, long long timestamp2) {
        return timestamp1 - timestamp2;
    }

    // Check if year is leap year
    inline bool isLeapYear(int year) {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

    // Get number of days in month
    inline int getDaysInMonth(int year, int month) {
        static const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (month < 1 || month > 12) return 0;
        if (month == 2 && isLeapYear(year)) return 29;
        return days[month - 1];
    }

    // Add duration to DateTime
    inline DateTime addDuration(const DateTime& dt, const Duration& duration) {
        long long timestamp = dateTimeToTimestamp(dt);
        timestamp += duration.seconds();
        return timestampToDateTime(timestamp, true);
    }

    // Calculate duration between two DateTimes
    inline Duration durationBetween(const DateTime& dt1, const DateTime& dt2) {
        long long ts1 = dateTimeToTimestamp(dt1);
        long long ts2 = dateTimeToTimestamp(dt2);
        return Duration((ts1 - ts2) * 1000);
    }

    // Timezone functions

    // Get timezone offset in seconds (local time - UTC)
    inline long long getTimezoneOffset() {
        time_t now = time(nullptr);
        struct tm local_tm, utc_tm;
        
    #ifdef _WIN32
        localtime_s(&local_tm, &now);
        gmtime_s(&utc_tm, &now);
    #else
        localtime_r(&now, &local_tm);
        gmtime_r(&now, &utc_tm);
    #endif
        
        // Convert both to timestamps and calculate difference
        time_t local_time = mktime(&local_tm);
        time_t utc_time = mktime(&utc_tm);
        
        return static_cast<long long>(difftime(local_time, utc_time));
    }

    // Convert local time to UTC
    inline DateTime localToUTC(const DateTime& local) {
        long long offset = getTimezoneOffset();
        long long timestamp = dateTimeToTimestamp(local) - offset;
        return timestampToDateTime(timestamp, false);
    }

    // Convert UTC to local time
    inline DateTime utcToLocal(const DateTime& utc) {
        long long offset = getTimezoneOffset();
        long long timestamp = dateTimeToTimestamp(utc) + offset;
        return timestampToDateTime(timestamp, true);
    }

    // Stopwatch class for easy time measurement

    class Stopwatch {
    private:
        Timer timer;
        std::vector<long long> laps;
        
    public:
        void start() {
            timer.start();
            laps.clear();
        }
        
        void stop() {
            timer.stop();
        }
        
        void reset() {
            timer.reset();
            laps.clear();
        }
        
        void lap() {
            laps.push_back(timer.elapsedMilliseconds());
        }
        
        long long elapsed() const {
            return timer.elapsedMilliseconds();
        }
        
        const std::vector<long long>& getLaps() const {
            return laps;
        }
        
        size_t lapCount() const {
            return laps.size();
        }
        
        long long getLap(size_t index) const {
            if (index < laps.size()) {
                return laps[index];
            }
            return 0;
        }
    };

    // Benchmark
    
    template<typename Func>
    inline double benchmark(Func&& func, int iterations = 1) {
        Timer timer;
        timer.start();
        
        for (int i = 0; i < iterations; ++i) {
            func();
        }
        
        timer.stop();
        return timer.elapsedSeconds() / iterations;
    }

    // Date/Time validation

    inline bool isValidDateTime(const DateTime& dt) {
        if (dt.year < 1900 || dt.year > 3000) return false;
        if (dt.month < 1 || dt.month > 12) return false;
        if (dt.day < 1 || dt.day > getDaysInMonth(dt.year, dt.month)) return false;
        if (dt.hour < 0 || dt.hour > 23) return false;
        if (dt.minute < 0 || dt.minute > 59) return false;
        if (dt.second < 0 || dt.second > 59) return false;
        if (dt.millisecond < 0 || dt.millisecond > 999) return false;
        if (dt.microsecond < 0 || dt.microsecond > 999) return false;
        return true;
    }
} // namespace TimeUtils