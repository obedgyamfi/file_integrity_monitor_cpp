#pragma once 

#include <windows.h>
#include <vector>
#include <string>

class Hasher {
    public:
        const char* buffer;

        DWORD hashObjectSize;
        DWORD cbData;
        BCRYPT_ALG_HANDLE hAlg;
        BCRYPT_HASH_HANDLE hHash;
        DWORD cbHash;
        std::vector<BYTE> hash;

        // constructor
        Hasher() {}

        // destructor
        ~Hasher() {}

        int hash(const char* data, ){}
}; // HASHER_HPP