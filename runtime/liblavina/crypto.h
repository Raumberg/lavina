#pragma once
#pragma comment(lib, "crypt32.lib")
#include "core.h"

#if defined(__unix__) || defined(__APPLE__)
inline std::string sha256(const std::string& input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, input.c_str(), input.length());
    SHA256_Final(hash, &sha256);
    
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

inline std::string md5(const std::string& input) {
    unsigned char hash[MD5_DIGEST_LENGTH];
    MD5_CTX md5;
    
    MD5_Init(&md5);
    MD5_Update(&md5, input.c_str(), input.length());
    MD5_Final(hash, &md5);
    
    std::stringstream ss;
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
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