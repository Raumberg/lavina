#pragma once
#include "core.h"
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>
#include <unordered_map>
#include <memory>

// ── Thread registry ────────────────────────────────────────────

static std::unordered_map<int64_t, std::unique_ptr<std::thread>> _lv_threads;
static std::atomic<int64_t> _lv_thread_counter{0};
static std::mutex _lv_thread_registry_mutex;

// Spawn a new thread running f().
// Caller MUST call __thread_wait() before the captured scope exits.
inline int64_t __thread_spawn(auto f) {
    int64_t id = ++_lv_thread_counter;
    auto task = std::make_shared<std::decay_t<decltype(f)>>(std::move(f));
    std::lock_guard<std::mutex> lock(_lv_thread_registry_mutex);
    _lv_threads[id] = std::make_unique<std::thread>([task]() { (*task)(); });
    return id;
}

// Wait for thread to complete.
// Named "wait" because "join" conflicts with try_remap_method in codegen.
inline void __thread_wait(int64_t id) {
    std::unique_ptr<std::thread> t;
    {
        std::lock_guard<std::mutex> lock(_lv_thread_registry_mutex);
        auto it = _lv_threads.find(id);
        if (it == _lv_threads.end()) return;
        t = std::move(it->second);
        _lv_threads.erase(it);
    }
    if (t && t->joinable()) {
        t->join();
    }
}

// Detach thread (let it run independently).
inline void __thread_detach(int64_t id) {
    std::lock_guard<std::mutex> lock(_lv_thread_registry_mutex);
    auto it = _lv_threads.find(id);
    if (it == _lv_threads.end()) return;
    if (it->second && it->second->joinable()) {
        it->second->detach();
    }
    _lv_threads.erase(it);
}

// Sleep current thread for ms milliseconds.
inline void __thread_sleep(int64_t ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

// Get current thread ID as int64_t.
inline int64_t __thread_current_id() {
    return static_cast<int64_t>(
        std::hash<std::thread::id>{}(std::this_thread::get_id())
    );
}

// ── Mutex registry ─────────────────────────────────────────────

static std::unordered_map<int64_t, std::unique_ptr<std::mutex>> _lv_mutexes;
static std::atomic<int64_t> _lv_mutex_counter{0};
static std::mutex _lv_mutex_registry_mutex;

inline int64_t __mutex_create() {
    int64_t id = ++_lv_mutex_counter;
    std::lock_guard<std::mutex> lock(_lv_mutex_registry_mutex);
    _lv_mutexes[id] = std::make_unique<std::mutex>();
    return id;
}

inline void __mutex_lock(int64_t id) {
    std::mutex* m = nullptr;
    {
        std::lock_guard<std::mutex> lock(_lv_mutex_registry_mutex);
        auto it = _lv_mutexes.find(id);
        if (it == _lv_mutexes.end()) return;
        m = it->second.get();
    }
    m->lock();
}

inline void __mutex_unlock(int64_t id) {
    std::mutex* m = nullptr;
    {
        std::lock_guard<std::mutex> lock(_lv_mutex_registry_mutex);
        auto it = _lv_mutexes.find(id);
        if (it == _lv_mutexes.end()) return;
        m = it->second.get();
    }
    m->unlock();
}

inline void __mutex_destroy(int64_t id) {
    std::lock_guard<std::mutex> lock(_lv_mutex_registry_mutex);
    _lv_mutexes.erase(id);
}

// ── Thread pool ────────────────────────────────────────────────

class _LvThreadPool {
public:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;

    _LvThreadPool(int64_t n) : stop(false) {
        for (int64_t i = 0; i < n; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->condition.wait(lock, [this] {
                            return this->stop || !this->tasks.empty();
                        });
                        if (this->stop && this->tasks.empty()) return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    void submit_task(std::function<void()> f) {
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            tasks.push(std::move(f));
        }
        condition.notify_one();
    }

    void shutdown() {
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            stop = true;
        }
        condition.notify_all();
        for (auto& w : workers) {
            if (w.joinable()) w.join();
        }
    }

    ~_LvThreadPool() {
        if (!stop) shutdown();
    }
};

static std::unordered_map<int64_t, std::unique_ptr<_LvThreadPool>> _lv_pools;
static std::atomic<int64_t> _lv_pool_counter{0};
static std::mutex _lv_pool_registry_mutex;

inline int64_t __pool_create(int64_t n) {
    int64_t id = ++_lv_pool_counter;
    std::lock_guard<std::mutex> lock(_lv_pool_registry_mutex);
    _lv_pools[id] = std::make_unique<_LvThreadPool>(n);
    return id;
}

inline void __pool_submit(int64_t id, auto f) {
    _LvThreadPool* pool = nullptr;
    {
        std::lock_guard<std::mutex> lock(_lv_pool_registry_mutex);
        auto it = _lv_pools.find(id);
        if (it == _lv_pools.end()) return;
        pool = it->second.get();
    }
    pool->submit_task(std::function<void()>(std::move(f)));
}

inline void __pool_shutdown(int64_t id) {
    std::unique_ptr<_LvThreadPool> pool;
    {
        std::lock_guard<std::mutex> lock(_lv_pool_registry_mutex);
        auto it = _lv_pools.find(id);
        if (it == _lv_pools.end()) return;
        pool = std::move(it->second);
        _lv_pools.erase(it);
    }
    pool->shutdown();
}
