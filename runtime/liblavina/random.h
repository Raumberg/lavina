#ifndef LAVINA_RANDOM_H
#define LAVINA_RANDOM_H

#include "core.h"

#pragma comment(lib, "bcrypt.lib")

#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif

#ifdef __cplusplus
extern "C" {
#endif

// PRNG state structure (xoshiro256**)
typedef struct RandPRNG {
    uint64_t state[4];
} RandPRNG;

// UUID structure
typedef struct RandUUID {
    uint8_t bytes[16];
} RandUUID;

static inline uint64_t _rand_prng_rotl(const uint64_t x, int k) {
    return (x << k) | (x >> (64 - k));
}

static inline uint64_t _rand_prng_next(RandPRNG *rng) {
    const uint64_t result = _rand_prng_rotl(rng->state[1] * 5, 7) * 9;
    const uint64_t t = rng->state[1] << 17;

    rng->state[2] ^= rng->state[0];
    rng->state[3] ^= rng->state[1];
    rng->state[1] ^= rng->state[2];
    rng->state[0] ^= rng->state[3];

    rng->state[2] ^= t;
    rng->state[3] = _rand_prng_rotl(rng->state[3], 45);

    return result;
}

// SplitMix64 for seeding
static inline uint64_t _rand_splitmix64(uint64_t *state) {
    uint64_t z = (*state += 0x9e3779b97f4a7c15ULL);
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
    z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
    return z ^ (z >> 31);
}

// Global PRNG instance and auto-initialization flag
static RandPRNG _rand_global_prng = {{0, 0, 0, 0}};
static bool _rand_global_prng_initialized = false;

// Seed the global PRNG
inline void rand_prng_seed(uint64_t seed) {
    uint64_t smstate = seed;
    _rand_global_prng.state[0] = _rand_splitmix64(&smstate);
    _rand_global_prng.state[1] = _rand_splitmix64(&smstate);
    _rand_global_prng.state[2] = _rand_splitmix64(&smstate);
    _rand_global_prng.state[3] = _rand_splitmix64(&smstate);
    _rand_global_prng_initialized = true;
}

// Auto-initialize global PRNG with time-based seed
static inline void _rand_prng_auto_init(void) {
    if (_rand_global_prng_initialized) return;
    
    uint64_t seed = (uint64_t)time(NULL);
    seed ^= (uint64_t)(uintptr_t)&seed;
    
    #ifdef RAND_PLATFORM_WINDOWS
        LARGE_INTEGER counter;
        QueryPerformanceCounter(&counter);
        seed ^= (uint64_t)counter.QuadPart;
    #else
        struct timespec ts;
        #ifdef CLOCK_MONOTONIC
            clock_gettime(CLOCK_MONOTONIC, &ts);
            seed ^= (uint64_t)ts.tv_nsec;
        #endif
    #endif
    
    rand_prng_seed(seed);
}

// Generate pseudo-random uint32_t
inline uint32_t rand_prng_u32(void) {
    _rand_prng_auto_init();
    return (uint32_t)(_rand_prng_next(&_rand_global_prng) >> 32);
}

// Generate pseudo-random uint64_t
inline uint64_t rand_prng_u64(void) {
    _rand_prng_auto_init();
    return _rand_prng_next(&_rand_global_prng);
}

// Generate pseudo-random int in range [min, max] (inclusive)
inline int rand_prng_range(int min, int max) {
    if (min > max) {
        int tmp = min;
        min = max;
        max = tmp;
    }
    if (min == max) return min;
    
    uint32_t range = (uint32_t)(max - min + 1);
    uint32_t limit = UINT32_MAX - (UINT32_MAX % range);
    uint32_t value;
    
    do {
        value = rand_prng_u32();
    } while (value >= limit);
    
    return min + (int)(value % range);
}

// Generate pseudo-random double in range [0.0, 1.0)
inline double rand_prng_double(void) {
    _rand_prng_auto_init();
    uint64_t x = _rand_prng_next(&_rand_global_prng) >> 11;
    return (double)x / (double)(1ULL << 53);
}

// Generate pseudo-random float in range [0.0f, 1.0f)
inline float rand_prng_float(void) {
    uint32_t x = rand_prng_u32() >> 8;
    return (float)x / (float)(1U << 24);
}

// Generate pseudo-random bool (50% probability)
inline bool rand_prng_bool(void) {
    return (rand_prng_u32() & 1) != 0;
}

// Fill buffer with pseudo-random bytes
inline void rand_prng_bytes(void *buf, size_t len) {
    if (!buf || len == 0) return;
    
    _rand_prng_auto_init();
    uint8_t *bytes = (uint8_t *)buf;
    size_t i = 0;
    
    while (i + 8 <= len) {
        uint64_t val = _rand_prng_next(&_rand_global_prng);
        memcpy(bytes + i, &val, 8);
        i += 8;
    }
    
    if (i < len) {
        uint64_t val = _rand_prng_next(&_rand_global_prng);
        memcpy(bytes + i, &val, len - i);
    }
}

// Create new PRNG instance with seed
inline RandPRNG* rand_prng_create(uint64_t seed) {
    RandPRNG *rng = (RandPRNG *)malloc(sizeof(RandPRNG));
    if (!rng) return NULL;
    
    uint64_t smstate = seed;
    rng->state[0] = _rand_splitmix64(&smstate);
    rng->state[1] = _rand_splitmix64(&smstate);
    rng->state[2] = _rand_splitmix64(&smstate);
    rng->state[3] = _rand_splitmix64(&smstate);
    
    return rng;
}

// Destroy PRNG instance
inline void rand_prng_destroy(RandPRNG *rng) {
    if (rng) {
        memset(rng, 0, sizeof(RandPRNG));
        free(rng);
    }
}

// Generate uint32_t from specific PRNG instance
inline uint32_t rand_prng_next_u32(RandPRNG *rng) {
    if (!rng) return 0;
    return (uint32_t)(_rand_prng_next(rng) >> 32);
}

// Generate uint64_t from specific PRNG instance
inline uint64_t rand_prng_next_u64(RandPRNG *rng) {
    if (!rng) return 0;
    return _rand_prng_next(rng);
}

// Generate int in range [min, max] from specific PRNG instance
inline int rand_prng_next_range(RandPRNG *rng, int min, int max) {
    if (!rng) return min;
    if (min > max) {
        int tmp = min;
        min = max;
        max = tmp;
    }
    if (min == max) return min;
    
    uint32_t range = (uint32_t)(max - min + 1);
    uint32_t limit = UINT32_MAX - (UINT32_MAX % range);
    uint32_t value;
    
    do {
        value = rand_prng_next_u32(rng);
    } while (value >= limit);
    
    return min + (int)(value % range);
}

// Generate double [0.0, 1.0) from specific PRNG instance
inline double rand_prng_next_double(RandPRNG *rng) {
    if (!rng) return 0.0;
    uint64_t x = _rand_prng_next(rng) >> 11;
    return (double)x / (double)(1ULL << 53);
}

// Fill buffer from specific PRNG instance
inline void rand_prng_next_bytes(RandPRNG *rng, void *buf, size_t len) {
    if (!rng || !buf || len == 0) return;
    
    uint8_t *bytes = (uint8_t *)buf;
    size_t i = 0;
    
    while (i + 8 <= len) {
        uint64_t val = _rand_prng_next(rng);
        memcpy(bytes + i, &val, 8);
        i += 8;
    }
    
    if (i < len) {
        uint64_t val = _rand_prng_next(rng);
        memcpy(bytes + i, &val, len - i);
    }
}

// Fill buffer with true random bytes from OS
inline bool rand_trng_bytes(void *buf, size_t len) {
    if (!buf || len == 0) return false;
    
#ifdef RAND_PLATFORM_WINDOWS
    // Windows: BCryptGenRandom (Vista+)
    NTSTATUS status = BCryptGenRandom(
        NULL,
        (PUCHAR)buf,
        (ULONG)len,
        BCRYPT_USE_SYSTEM_PREFERRED_RNG
    );
    return status == 0;
    
#elif defined(RAND_PLATFORM_MACOS)
    // macOS: arc4random_buf (never fails)
    arc4random_buf(buf, len);
    return true;
    
#elif defined(RAND_PLATFORM_LINUX) || defined(RAND_PLATFORM_UNIX)
    // Linux/Unix: /dev/urandom
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0) return false;
    
    size_t total = 0;
    while (total < len) {
        ssize_t n = read(fd, (uint8_t *)buf + total, len - total);
        if (n <= 0) {
            close(fd);
            return false;
        }
        total += (size_t)n;
    }
    
    close(fd);
    return true;
    
#else
    return false;
#endif
}

// Generate true random uint32_t
inline uint32_t rand_trng_u32(void) {
    uint32_t val = 0;
    rand_trng_bytes(&val, sizeof(val));
    return val;
}

// Generate true random uint64_t
inline uint64_t rand_trng_u64(void) {
    uint64_t val = 0;
    rand_trng_bytes(&val, sizeof(val));
    return val;
}

// Generate true random int in range [min, max]
inline int rand_trng_range(int min, int max) {
    if (min > max) {
        int tmp = min;
        min = max;
        max = tmp;
    }
    if (min == max) return min;
    
    uint32_t range = (uint32_t)(max - min + 1);
    uint32_t limit = UINT32_MAX - (UINT32_MAX % range);
    uint32_t value;
    
    do {
        value = rand_trng_u32();
    } while (value >= limit);
    
    return min + (int)(value % range);
}

// Generate true random double [0.0, 1.0)
inline double rand_trng_double(void) {
    uint64_t x = rand_trng_u64() >> 11;
    return (double)x / (double)(1ULL << 53);
}

// Fisher-Yates shuffle
inline void rand_prng_shuffle(void *arr, size_t count, size_t elem_size) {
    if (!arr || count <= 1 || elem_size == 0) return;
    
    _rand_prng_auto_init();
    
    uint8_t *array = (uint8_t *)arr;
    uint8_t *temp = (uint8_t *)malloc(elem_size);
    if (!temp) return;
    
    for (size_t i = count - 1; i > 0; i--) {
        size_t j = (size_t)rand_prng_range(0, (int)i);
        
        memcpy(temp, array + i * elem_size, elem_size);
        memcpy(array + i * elem_size, array + j * elem_size, elem_size);
        memcpy(array + j * elem_size, temp, elem_size);
    }
    
    free(temp);
}

// Choose random element from array
inline void* rand_prng_choice(void *arr, size_t count, size_t elem_size) {
    if (!arr || count == 0 || elem_size == 0) return NULL;
    
    size_t idx = (size_t)rand_prng_range(0, (int)(count - 1));
    return (uint8_t *)arr + idx * elem_size;
}

// Coin flip with probability [0.0, 1.0]
inline bool rand_prng_chance(double probability) {
    if (probability <= 0.0) return false;
    if (probability >= 1.0) return true;
    return rand_prng_double() < probability;
}

// Normal distribution (Box-Muller transform)
inline double rand_prng_normal(double mean, double stddev) {
    double u1 = rand_prng_double();
    double u2 = rand_prng_double();
    
    if (u1 < 1e-10) u1 = 1e-10;
    
    double z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * 3.14159265358979323846 * u2);
    return mean + z0 * stddev;
}

// Exponential distribution
inline double rand_prng_exponential(double lambda) {
    if (lambda <= 0.0) return 0.0;
    double u = rand_prng_double();
    if (u < 1e-10) u = 1e-10;
    return -log(u) / lambda;
}

// Generate UUID v4 using TRNG
inline RandUUID rand_trng_uuid(void) {
    RandUUID uuid;
    rand_trng_bytes(uuid.bytes, 16);
    
    uuid.bytes[6] = (uuid.bytes[6] & 0x0F) | 0x40;
    uuid.bytes[8] = (uuid.bytes[8] & 0x3F) | 0x80;
    
    return uuid;
}

// Convert UUID to string "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"
inline void rand_uuid_to_string(RandUUID uuid, char *out) {
    static const char hex[] = "0123456789abcdef";
    int pos = 0;
    
    for (int i = 0; i < 16; i++) {
        if (i == 4 || i == 6 || i == 8 || i == 10) {
            out[pos++] = '-';
        }
        out[pos++] = hex[(uuid.bytes[i] >> 4) & 0xF];
        out[pos++] = hex[uuid.bytes[i] & 0xF];
    }
    out[pos] = '\0';
}

// ============================================================================
// Convenience Aliases (shorter names)
// ============================================================================

#define rand_seed(s)          rand_prng_seed(s)
#define rand_u32()            rand_prng_u32()
#define rand_u64()            rand_prng_u64()
#define rand_int(a, b)        rand_prng_range(a, b)
#define rand_range(a, b)      rand_prng_range(a, b)
#define rand_double()         rand_prng_double()
#define rand_float()          rand_prng_float()
#define rand_bool()           rand_prng_bool()
#define rand_bytes(b, l)      rand_prng_bytes(b, l)
#define rand_shuffle(a, c, s) rand_prng_shuffle(a, c, s)
#define rand_choice(a, c, s)  rand_prng_choice(a, c, s)
#define rand_chance(p)        rand_prng_chance(p)
#define rand_normal(m, s)     rand_prng_normal(m, s)

#define rand_secure_bytes(b, l) rand_trng_bytes(b, l)
#define rand_secure_u32()       rand_trng_u32()
#define rand_secure_u64()       rand_trng_u64()
#define rand_uuid()             rand_trng_uuid()

#ifdef __cplusplus
}
#endif

#endif // LAVINA_RANDOM_H
