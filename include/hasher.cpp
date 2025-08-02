#include "hasher.hpp"
#include <bcrypt.h>
#include <iostream>


#pragma comment(lib, "bcrypt.lib") // MSVC-specific, but okay to leave in


class Hasher {
    public:
        const char* buffer;

        DWORD hashObjectSize;
        DWORD cbData;
        BCRYPT_ALG_HANDLE hAlg;
        BCRYPT_HASH_HANDLE hHash;
        PUCHAR pbHashObject = nullptr;
        DWORD cbHash;
        std::vector<BYTE> hash;

        // constructor
        Hasher() : 
            hashObjectSize(0), 
            cbData(0),
            hAlg(nullptr), 
            hHash(nullptr),
            pbHashObject(nullptr), 
            cbHash(0) 
            {}

        // destructor
        ~Hasher() {
            if(hHash) BCryptDestroyHash(hHash);
            if(hAlg) BCryptCloseAlgorithmProvider(hAlg, 0);
            if(pbHashObject) delete[] pbHashObject;
        }

        int hash(const char* data){
            if (BCryptOpenAlgorithmProvider(&hAlg, BCRYPT_SHA256_ALGORITHM, NULL, 0) != 0)
            {
                std::cerr << "Failed to open algorithm provider.\n";
                hAlg = nullptr;
                return 1;
            }

            if (BCryptGetProperty(hAlg, BCRYPT_OBJECT_LENGTH, (PUCHAR)&hashObjectSize, sizeof(DWORD), &cbData, 0) != 0 ||
                BCryptGetProperty(hAlg, BCRYPT_HASH_LENGTH, (PUCHAR)&cbHash, sizeof(DWORD), &cbData, 0) != 0)
            {
                std::cerr << "Failed to get hash properties.\n";
                BCryptCloseAlgorithmProvider(hAlg, 0);
                hAlg = nullptr;
                return 1;
            }

            pbHashObject = new UCHAR[hashObjectSize];
            hash.resize(cbHash);

            if (BCryptCreateHash(hAlg, &hHash, pbHashObject, hashObjectSize, NULL, 0, 0) != 0 ||
                BCryptHashData(hHash, (PUCHAR)data, strlen(data), 0) != 0 ||
                BCryptFinishHash(hHash, hash.data(), cbHash, 0) != 0)
            {
                std::cerr << "Hashing failed.\n";
                if(hHash) BCryptDestroyHash(hHash);
                hHash = nullptr; 
                delete[] pbHashObject;
                pbHashObject = nullptr;
                BCryptCloseAlgorithmProvider(hAlg, 0);
                hAlg = nullptr;
                return 1;
            }

            for (BYTE b : hash)
                printf("%02x", b);
            printf("\n");

            if(hHash) BCryptDestroyHash(hHash);
            hHash = nullptr;
            delete[] pbHashObject;
            pbHashObject = nullptr;
            if(hAlg) BCryptCloseAlgorithmProvider(hAlg, 0);
            hAlg = nullptr;

            return 0;
        }
}; // HASHER_HPP

