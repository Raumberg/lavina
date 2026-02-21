#pragma once
#include "core.h"

using lv_bytes = std::vector<uint8_t>;

// ── Construction ────────────────────────────────────────────

inline lv_bytes __bytes_create(int64_t size) {
    return lv_bytes(static_cast<size_t>(size), 0);
}

inline lv_bytes __bytes_from_string(const std::string& s) {
    return lv_bytes(s.begin(), s.end());
}

inline std::string __bytes_to_string(const lv_bytes& b) {
    return std::string(b.begin(), b.end());
}

// ── Hex encoding/decoding ───────────────────────────────────

inline std::string __bytes_to_hex(const lv_bytes& b) {
    static const char hex_chars[] = "0123456789abcdef";
    std::string result;
    result.reserve(b.size() * 2);
    for (uint8_t byte : b) {
        result.push_back(hex_chars[byte >> 4]);
        result.push_back(hex_chars[byte & 0x0F]);
    }
    return result;
}

inline lv_bytes __bytes_from_hex(const std::string& hex) {
    if (hex.size() % 2 != 0) {
        throw std::runtime_error("from_hex: odd-length hex string");
    }
    lv_bytes result;
    result.reserve(hex.size() / 2);
    for (size_t i = 0; i < hex.size(); i += 2) {
        uint8_t hi = 0, lo = 0;
        char c1 = hex[i], c2 = hex[i + 1];
        if (c1 >= '0' && c1 <= '9') hi = c1 - '0';
        else if (c1 >= 'a' && c1 <= 'f') hi = c1 - 'a' + 10;
        else if (c1 >= 'A' && c1 <= 'F') hi = c1 - 'A' + 10;
        else throw std::runtime_error("from_hex: invalid hex char");
        if (c2 >= '0' && c2 <= '9') lo = c2 - '0';
        else if (c2 >= 'a' && c2 <= 'f') lo = c2 - 'a' + 10;
        else if (c2 >= 'A' && c2 <= 'F') lo = c2 - 'A' + 10;
        else throw std::runtime_error("from_hex: invalid hex char");
        result.push_back((hi << 4) | lo);
    }
    return result;
}

// ── Byte-level access ───────────────────────────────────────

inline int64_t __bytes_get(const lv_bytes& b, int64_t idx) {
    if (idx < 0 || idx >= static_cast<int64_t>(b.size())) {
        throw std::runtime_error("bytes: index out of range");
    }
    return static_cast<int64_t>(b[static_cast<size_t>(idx)]);
}

inline void __bytes_set(lv_bytes& b, int64_t idx, int64_t val) {
    if (idx < 0 || idx >= static_cast<int64_t>(b.size())) {
        throw std::runtime_error("bytes: index out of range");
    }
    b[static_cast<size_t>(idx)] = static_cast<uint8_t>(val & 0xFF);
}

// ── Multi-byte read/write (big-endian) ──────────────────────

inline int64_t __bytes_read_u16_be(const lv_bytes& b, int64_t offset) {
    size_t o = static_cast<size_t>(offset);
    if (o + 2 > b.size()) throw std::runtime_error("bytes: read_u16_be out of range");
    return static_cast<int64_t>((static_cast<uint16_t>(b[o]) << 8) | b[o + 1]);
}

inline void __bytes_write_u16_be(lv_bytes& b, int64_t offset, int64_t val) {
    size_t o = static_cast<size_t>(offset);
    if (o + 2 > b.size()) throw std::runtime_error("bytes: write_u16_be out of range");
    uint16_t v = static_cast<uint16_t>(val);
    b[o] = static_cast<uint8_t>(v >> 8);
    b[o + 1] = static_cast<uint8_t>(v & 0xFF);
}

inline int64_t __bytes_read_u32_be(const lv_bytes& b, int64_t offset) {
    size_t o = static_cast<size_t>(offset);
    if (o + 4 > b.size()) throw std::runtime_error("bytes: read_u32_be out of range");
    return static_cast<int64_t>(
        (static_cast<uint32_t>(b[o]) << 24) |
        (static_cast<uint32_t>(b[o + 1]) << 16) |
        (static_cast<uint32_t>(b[o + 2]) << 8) |
        b[o + 3]
    );
}

inline void __bytes_write_u32_be(lv_bytes& b, int64_t offset, int64_t val) {
    size_t o = static_cast<size_t>(offset);
    if (o + 4 > b.size()) throw std::runtime_error("bytes: write_u32_be out of range");
    uint32_t v = static_cast<uint32_t>(val);
    b[o] = static_cast<uint8_t>(v >> 24);
    b[o + 1] = static_cast<uint8_t>((v >> 16) & 0xFF);
    b[o + 2] = static_cast<uint8_t>((v >> 8) & 0xFF);
    b[o + 3] = static_cast<uint8_t>(v & 0xFF);
}

// ── Multi-byte read/write (little-endian) ───────────────────

inline int64_t __bytes_read_u16_le(const lv_bytes& b, int64_t offset) {
    size_t o = static_cast<size_t>(offset);
    if (o + 2 > b.size()) throw std::runtime_error("bytes: read_u16_le out of range");
    return static_cast<int64_t>(b[o] | (static_cast<uint16_t>(b[o + 1]) << 8));
}

inline void __bytes_write_u16_le(lv_bytes& b, int64_t offset, int64_t val) {
    size_t o = static_cast<size_t>(offset);
    if (o + 2 > b.size()) throw std::runtime_error("bytes: write_u16_le out of range");
    uint16_t v = static_cast<uint16_t>(val);
    b[o] = static_cast<uint8_t>(v & 0xFF);
    b[o + 1] = static_cast<uint8_t>(v >> 8);
}

inline int64_t __bytes_read_u32_le(const lv_bytes& b, int64_t offset) {
    size_t o = static_cast<size_t>(offset);
    if (o + 4 > b.size()) throw std::runtime_error("bytes: read_u32_le out of range");
    return static_cast<int64_t>(
        b[o] |
        (static_cast<uint32_t>(b[o + 1]) << 8) |
        (static_cast<uint32_t>(b[o + 2]) << 16) |
        (static_cast<uint32_t>(b[o + 3]) << 24)
    );
}

inline void __bytes_write_u32_le(lv_bytes& b, int64_t offset, int64_t val) {
    size_t o = static_cast<size_t>(offset);
    if (o + 4 > b.size()) throw std::runtime_error("bytes: write_u32_le out of range");
    uint32_t v = static_cast<uint32_t>(val);
    b[o] = static_cast<uint8_t>(v & 0xFF);
    b[o + 1] = static_cast<uint8_t>((v >> 8) & 0xFF);
    b[o + 2] = static_cast<uint8_t>((v >> 16) & 0xFF);
    b[o + 3] = static_cast<uint8_t>(v >> 24);
}

// ── Slice / Concat / Compare ────────────────────────────────

inline lv_bytes __bytes_slice(const lv_bytes& b, int64_t start, int64_t end) {
    if (start < 0) start = 0;
    if (end > static_cast<int64_t>(b.size())) end = b.size();
    if (start >= end) return {};
    return lv_bytes(b.begin() + start, b.begin() + end);
}

inline lv_bytes __bytes_concat(const lv_bytes& a, const lv_bytes& b) {
    lv_bytes result;
    result.reserve(a.size() + b.size());
    result.insert(result.end(), a.begin(), a.end());
    result.insert(result.end(), b.begin(), b.end());
    return result;
}

inline bool __bytes_equals(const lv_bytes& a, const lv_bytes& b) {
    return a == b;
}

inline void __bytes_fill(lv_bytes& b, int64_t val) {
    std::fill(b.begin(), b.end(), static_cast<uint8_t>(val & 0xFF));
}

// ── Print support ───────────────────────────────────────────

inline std::string to_string(const lv_bytes& b) {
    return "bytes[" + std::to_string(b.size()) + "]";
}

inline void print(const lv_bytes& b) {
    std::cout << "bytes[" << b.size() << "](";
    static const char hx[] = "0123456789abcdef";
    for (size_t i = 0; i < b.size() && i < 32; i++) {
        if (i > 0) std::cout << " ";
        std::cout << hx[b[i] >> 4] << hx[b[i] & 0x0F];
    }
    if (b.size() > 32) std::cout << "...";
    std::cout << ")";
}

inline void println(const lv_bytes& b) {
    print(b);
    std::cout << std::endl;
}

inline std::string operator+(const std::string& s, const lv_bytes& b) {
    return s + to_string(b);
}

inline std::string operator+(const lv_bytes& b, const std::string& s) {
    return to_string(b) + s;
}
