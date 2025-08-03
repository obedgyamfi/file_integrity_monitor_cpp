#pragma once 

#include <windows.h>
#include <vector>
#include <string>

class Hasher {
    public:
        DWORD hashObjectSize;
        DWORD cbData;
        BCRYPT_ALG_HANDLE hAlg;
        BCRYPT_HASH_HANDLE hHash;
        PUCHAR pbHashObject;
        DWORD cbHash;
        // std::vector<BYTE> hash;

        // constructor
        Hasher();

        // destructor
        ~Hasher();

        int createHash(const char* fileContent, std::vector<BYTE>& hash);
}; // HASHER_HPP