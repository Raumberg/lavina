#pragma once
#pragma comment(lib, "crypt32.lib")

#include "core.h"

#if defined(__unix__) || defined(__APPLE__)
#define ROTR(x, n) (((x) >> (n)) | ((x) << (32 - (n))))
#define SHR(x, n) ((x) >> (n))

#define CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))

#define SIGMA0(x) (ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define SIGMA1(x) (ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25))
#define GAMMA0(x) (ROTR(x, 7) ^ ROTR(x, 18) ^ SHR(x, 3))
#define GAMMA1(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ SHR(x, 10))

static const uint32_t SHA256_K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

static const uint32_t SHA256_H0[8] = {
    0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
    0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

typedef struct {
    uint8_t data[64];
    uint32_t datalen;
    uint64_t bitlen;
    uint32_t state[8];
} SHA256_CTX;

static inline void uint32_to_be(uint32_t value, uint8_t* bytes) {
    bytes[0] = (value >> 24) & 0xFF;
    bytes[1] = (value >> 16) & 0xFF;
    bytes[2] = (value >> 8) & 0xFF;
    bytes[3] = value & 0xFF;
}

static inline void uint64_to_be(uint64_t value, uint8_t* bytes) {
    for (int i = 7; i >= 0; i--) {
        bytes[i] = value & 0xFF;
        value >>= 8;
    }
}

// Initialization: ctx->datalen = 0; ctx->bitlen = 0;
// memcpy(ctx->state, SHA256_H0, sizeof(ctx->state));
inline void sha256_init(SHA256_CTX* ctx) {
    ctx->datalen = 0;
    ctx->bitlen = 0;
    memcpy(ctx->state, SHA256_H0, sizeof(ctx->state));
}

static inline void sha256_transform(SHA256_CTX* ctx, const uint8_t data[]) {
    uint32_t w[64];
    uint32_t a, b, c, d, e, f, g, h;
    uint32_t t1, t2;
    
    for (int i = 0; i < 16; i++) {
        w[i] = (data[i*4] << 24) | (data[i*4+1] << 16) | 
               (data[i*4+2] << 8) | (data[i*4+3]);
    }
    
    for (int i = 16; i < 64; i++) {
        w[i] = GAMMA1(w[i-2]) + w[i-7] + GAMMA0(w[i-15]) + w[i-16];
    }
    
    a = ctx->state[0];
    b = ctx->state[1];
    c = ctx->state[2];
    d = ctx->state[3];
    e = ctx->state[4];
    f = ctx->state[5];
    g = ctx->state[6];
    h = ctx->state[7];
    
    for (int i = 0; i < 64; i++) {
        t1 = h + SIGMA1(e) + CH(e, f, g) + SHA256_K[i] + w[i];
        t2 = SIGMA0(a) + MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }
    
    ctx->state[0] += a;
    ctx->state[1] += b;
    ctx->state[2] += c;
    ctx->state[3] += d;
    ctx->state[4] += e;
    ctx->state[5] += f;
    ctx->state[6] += g;
    ctx->state[7] += h;
}

// ctx: SHA256_CTX, data: input data, len: length of input data
inline void sha256_update(SHA256_CTX* ctx, const uint8_t data[], size_t len) {
    for (size_t i = 0; i < len; i++) {
        ctx->data[ctx->datalen] = data[i];
        ctx->datalen++;
        
        if (ctx->datalen == 64) {
            sha256_transform(ctx, ctx->data);
            ctx->bitlen += 512;
            ctx->datalen = 0;
        }
    }
}

// ctx: SHA256_CTX, hash: output buffer (32 bytes)
inline void sha256_final(SHA256_CTX* ctx, uint8_t hash[]) {
    size_t i = ctx->datalen;
    
    if (ctx->datalen < 56) {
        ctx->data[i++] = 0x80;
        
        while (i < 56) {
            ctx->data[i++] = 0x00;
        }
    } else {
        ctx->data[i++] = 0x80;
        
        while (i < 64) {
            ctx->data[i++] = 0x00;
        }
        
        sha256_transform(ctx, ctx->data);
        memset(ctx->data, 0, 56);
    }
    
    ctx->bitlen += ctx->datalen * 8;
    uint64_to_be(ctx->bitlen, ctx->data + 56);
    
    sha256_transform(ctx, ctx->data);
    
    for (i = 0; i < 8; i++) {
        uint32_to_be(ctx->state[i], hash + i * 4);
    }
}

inline std::string sha256(const std::string& input) {
    SHA256_CTX ctx;
    uint8_t hash[32];
    std::stringstream ss;
    
    sha256_init(&ctx);
    sha256_update(&ctx, (uint8_t*)input.c_str(), input.length());
    sha256_final(&ctx, hash);
    
    for (int i = 0; i < 32; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    
    return ss.str();
}

static std::string sha256_from_cstr(const char* input) {
    return sha256(std::string(input));
}

/* -------------------------MD5--------------------------- */

static const uint8_t MD5_S[64] = {
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
    5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20,
    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

static const uint32_t MD5_K[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

static const uint32_t MD5_H0[4] = {
    0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476
};

#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

#define FF(a, b, c, d, x, s, ac) { \
    (a) += F((b), (c), (d)) + (x) + (ac); \
    (a) = ROTATE_LEFT((a), (s)); \
    (a) += (b); \
}
#define GG(a, b, c, d, x, s, ac) { \
    (a) += G((b), (c), (d)) + (x) + (ac); \
    (a) = ROTATE_LEFT((a), (s)); \
    (a) += (b); \
}
#define HH(a, b, c, d, x, s, ac) { \
    (a) += H((b), (c), (d)) + (x) + (ac); \
    (a) = ROTATE_LEFT((a), (s)); \
    (a) += (b); \
}
#define II(a, b, c, d, x, s, ac) { \
    (a) += I((b), (c), (d)) + (x) + (ac); \
    (a) = ROTATE_LEFT((a), (s)); \
    (a) += (b); \
}

typedef struct {
    uint8_t data[64];
    uint32_t datalen;
    uint64_t bitlen;
    uint32_t state[4];
} MD5_CTX;

static inline void uint32_to_le(uint32_t value, uint8_t* bytes) {
    bytes[0] = value & 0xFF;
    bytes[1] = (value >> 8) & 0xFF;
    bytes[2] = (value >> 16) & 0xFF;
    bytes[3] = (value >> 24) & 0xFF;
}

static inline void uint64_to_le(uint64_t value, uint8_t* bytes) {
    for (int i = 0; i < 8; i++) {
        bytes[i] = value & 0xFF;
        value >>= 8;
    }
}

inline void md5_init(MD5_CTX* ctx) {
    ctx->datalen = 0;
    ctx->bitlen = 0;
    memcpy(ctx->state, MD5_H0, sizeof(ctx->state));
}

static void md5_transform(MD5_CTX* ctx, const uint8_t data[]) {
    uint32_t a, b, c, d, i;
    uint32_t x[16];
    
    for (i = 0; i < 16; i++) {
        x[i] = data[i*4] | (data[i*4+1] << 8) | 
               (data[i*4+2] << 16) | (data[i*4+3] << 24);
    }
    
    a = ctx->state[0];
    b = ctx->state[1];
    c = ctx->state[2];
    d = ctx->state[3];
    
    for (i = 0; i < 16; i++) {
        FF(a, b, c, d, x[i], MD5_S[i], MD5_K[i]);
        uint32_t temp = d; d = c; c = b; b = a; a = temp;
    }
    
    for (i = 16; i < 32; i++) {
        GG(a, b, c, d, x[(5*i + 1) % 16], MD5_S[i], MD5_K[i]);
        uint32_t temp = d; d = c; c = b; b = a; a = temp;
    }
    
    for (i = 32; i < 48; i++) {
        HH(a, b, c, d, x[(3*i + 5) % 16], MD5_S[i], MD5_K[i]);
        uint32_t temp = d; d = c; c = b; b = a; a = temp;
    }
    
    for (i = 48; i < 64; i++) {
        II(a, b, c, d, x[(7*i) % 16], MD5_S[i], MD5_K[i]);
        uint32_t temp = d; d = c; c = b; b = a; a = temp;
    }
    
    ctx->state[0] += a;
    ctx->state[1] += b;
    ctx->state[2] += c;
    ctx->state[3] += d;
}

inline void md5_update(MD5_CTX* ctx, const uint8_t data[], size_t len) {
    for (size_t i = 0; i < len; i++) {
        ctx->data[ctx->datalen] = data[i];
        ctx->datalen++;
        
        if (ctx->datalen == 64) {
            md5_transform(ctx, ctx->data);
            ctx->bitlen += 512;
            ctx->datalen = 0;
        }
    }
}

inline void md5_final(MD5_CTX* ctx, uint8_t hash[]) {
    size_t i = ctx->datalen;
    
    if (ctx->datalen < 56) {
        ctx->data[i++] = 0x80;
        
        while (i < 56) {
            ctx->data[i++] = 0x00;
        }
    } else {
        ctx->data[i++] = 0x80;
        
        while (i < 64) {
            ctx->data[i++] = 0x00;
        }
        
        md5_transform(ctx, ctx->data);
        memset(ctx->data, 0, 56);
    }
    
    ctx->bitlen += ctx->datalen * 8;
    uint64_to_le(ctx->bitlen, ctx->data + 56);

    md5_transform(ctx, ctx->data);
    
    for (i = 0; i < 4; i++) {
        uint32_to_le(ctx->state[i], hash + i * 4);
    }
}

inline std::string md5(const std::string& input) {
    MD5_CTX ctx;
    uint8_t hash[16];
    std::stringstream ss;
    
    md5_init(&ctx);
    md5_update(&ctx, (uint8_t*)input.c_str(), input.length());
    md5_final(&ctx, hash);
    
    for (int i = 0; i < 16; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    
    return ss.str();
}

static int is_little_endian() {
    uint32_t test = 1;
    return *(uint8_t*)&test == 1;
}
#endif

#ifdef _WIN32
inline std::string sha256(const std::string& input) {
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    BYTE hash[32];
    DWORD hashLen = 32;
    std::stringstream ss;
    
    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
        return "Failed to acquire crypto context.";
    }
    
    if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)) {
        CryptReleaseContext(hProv, 0);
        return "Failed to create hash object.";
    }
    
    if (!CryptHashData(hHash, (BYTE*)input.c_str(), input.length(), 0)) {
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return "Failed to hash data.";
    }
    
    if (!CryptGetHashParam(hHash, HP_HASHVAL, hash, &hashLen, 0)) {
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return "Failed to get hash.";
    }
    
    for (DWORD i = 0; i < hashLen; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    
    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);
    
    return ss.str();
}

inline std::string md5(const std::string& input) {
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    BYTE hash[16];
    DWORD hashLen = 16;
    std::stringstream ss;
    
    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
        return "Failed to acquire crypto context.";
    }
    
    if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {
        CryptReleaseContext(hProv, 0);
        return "Failed to create hash object.";
    }
    
    if (!CryptHashData(hHash, (BYTE*)input.c_str(), input.length(), 0)) {
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return "Failed to hash data.";
    }
    
    if (!CryptGetHashParam(hHash, HP_HASHVAL, hash, &hashLen, 0)) {
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return "Failed to get hash.";
    }
    
    for (DWORD i = 0; i < hashLen; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    
    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);
    
    return ss.str();
}
#endif