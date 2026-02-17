#pragma once
#include "core.h"
#include "bytes.h"
#include <random>

// ── UUID v4 support ─────────────────────────────────────────
// UUID is represented as 16 bytes (lv_bytes).
// String format: "550e8400-e29b-41d4-a716-446655440000"

// Parse UUID string (with or without dashes) → 16 bytes
inline lv_bytes __uuid_parse(const std::string& s) {
    // Strip dashes
    std::string hex;
    hex.reserve(32);
    for (char c : s) {
        if (c != '-') hex.push_back(c);
    }
    if (hex.size() != 32) {
        throw std::runtime_error("uuid_parse: invalid UUID string (expected 32 hex chars, got " + std::to_string(hex.size()) + ")");
    }
    return __bytes_from_hex(hex);
}

// Format 16 bytes → UUID string with dashes
inline std::string __uuid_to_string(const lv_bytes& b) {
    if (b.size() != 16) {
        throw std::runtime_error("uuid_to_string: expected 16 bytes, got " + std::to_string(b.size()));
    }
    std::string hex = __bytes_to_hex(b);
    // Insert dashes: 8-4-4-4-12
    return hex.substr(0, 8) + "-" + hex.substr(8, 4) + "-" +
           hex.substr(12, 4) + "-" + hex.substr(16, 4) + "-" +
           hex.substr(20, 12);
}

// Generate random UUID v4
inline lv_bytes __uuid_random() {
    static thread_local std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, 255);

    lv_bytes uuid(16);
    for (int i = 0; i < 16; i++) {
        uuid[i] = static_cast<uint8_t>(dist(rng));
    }
    // Set version 4: byte 6 high nibble = 0100
    uuid[6] = (uuid[6] & 0x0F) | 0x40;
    // Set variant 1: byte 8 high bits = 10xx
    uuid[8] = (uuid[8] & 0x3F) | 0x80;
    return uuid;
}

// Compare two UUIDs
inline bool __uuid_equals(const lv_bytes& a, const lv_bytes& b) {
    if (a.size() != 16 || b.size() != 16) return false;
    return a == b;
}
