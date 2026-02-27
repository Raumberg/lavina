#pragma once
#include "core.h"

static const std::string base64_chars = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

// Lookup table for decoding
static constexpr std::array<int, 256> init_base64_lookup() {
    std::array<int, 256> lookup{};
    for (int i = 0; i < 256; i++) {
        lookup[i] = -1;
    }
    for (size_t i = 0; i < 64; i++) {
        lookup[static_cast<unsigned char>("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i])] = i;
    }
    return lookup;
}

static constexpr auto base64_lookup = init_base64_lookup();

inline std::string base64_encode(const std::string& input) {
    std::string output;
    output.reserve(((input.length() + 2) / 3) * 4);
    
    int i = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    
    for (char c : input) {
        char_array_3[i++] = c;
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;
            
            for (int j = 0; j < 4; j++) {
                output += base64_chars[char_array_4[j]];
            }
            i = 0;
        }
    }
    
    if (i > 0) {
        for (int j = i; j < 3; j++) {
            char_array_3[j] = '\0';
        }
        
        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        
        for (int j = 0; j < i + 1; j++) {
            output += base64_chars[char_array_4[j]];
        }
        
        while (i++ < 3) {
            output += '=';
        }
    }
    
    return output;
}

inline std::string base64_decode(const std::string& input) {
    std::string output;
    output.reserve((input.length() / 4) * 3);
    
    int i = 0;
    unsigned char char_array_4[4], char_array_3[3];
    
    for (char c : input) {
        if (c == '=') {
            break;
        }
        
        int val = base64_lookup[static_cast<unsigned char>(c)];
        if (val == -1) {
            continue;
        }
        
        char_array_4[i++] = static_cast<unsigned char>(val);
        if (i == 4) {
            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0x0f) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x03) << 6) + char_array_4[3];
            
            for (int j = 0; j < 3; j++) {
                output += char_array_3[j];
            }
            i = 0;
        }
    }
    
    if (i > 0) {
        for (int j = i; j < 4; j++) {
            char_array_4[j] = 0;
        }
        
        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0x0f) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        
        for (int j = 0; j < i - 1; j++) {
            output += char_array_3[j];
        }
    }
    
    return output;
}

// URL-safe Base64 variants
inline std::string base64_url_encode(const std::string& input) {
    std::string encoded = base64_encode(input);
    
    // Replace characters and remove padding
    for (char& c : encoded) {
        if (c == '+') c = '-';
        else if (c == '/') c = '_';
    }
    
    // Remove padding (optional, depends on your requirements)
    size_t pad_pos = encoded.find('=');
    if (pad_pos != std::string::npos) {
        encoded.erase(pad_pos);
    }
    
    return encoded;
}

inline std::string base64_url_decode(const std::string& input) {
    std::string normalized = input;
    
    for (char& c : normalized) {
        if (c == '-') c = '+';
        else if (c == '_') c = '/';
    }
    
    size_t pad = (4 - (normalized.length() % 4)) % 4;
    normalized.append(pad, '=');
    
    return base64_decode(normalized);
}

inline bool base64_is_valid(const std::string& input) {
    if (input.empty()) {
        return true;
    }
    
    if (input.length() % 4 != 0) {
        return false;
    }
    
    size_t padding_count = 0;
    bool padding_started = false;
    
    for (size_t i = 0; i < input.length(); i++) {
        char c = input[i];
        
        if (c == '=') {
            padding_started = true;
            padding_count++;
            if (padding_count > 2) {
                return false;
            }
        } else {
            if (padding_started) {
                return false;
            }
            if (base64_chars.find(c) == std::string::npos) {
                return false;
            }
        }
    }
    
    return true;
}

// Overloads for binary data
inline std::string base64_encode_binary(const std::vector<unsigned char>& input) {
    return base64_encode(std::string(input.begin(), input.end()));
}

inline std::vector<unsigned char> base64_decode_binary(const std::string& input) {
    std::string decoded = base64_decode(input);
    return std::vector<unsigned char>(decoded.begin(), decoded.end());
}