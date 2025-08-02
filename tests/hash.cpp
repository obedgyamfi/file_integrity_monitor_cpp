#include <windows.h>
#include <iostream>
#include <vector> 
#include <bcrypt.h>

int main()
{
    const char *data = "hello world, i'm just testing if the code i wrote to use openssl cryptographic function sha256 to hash the contents of a file is working as expected";
    DWORD hashObjectSize = 0, cbData = 0;
    BCRYPT_ALG_HANDLE hAlg = nullptr;
    BCRYPT_HASH_HANDLE hHash = nullptr;
    PUCHAR pbHashObject = nullptr;
    DWORD cbHash = 0;
    std::vector<BYTE> hash;

    if (BCryptOpenAlgorithmProvider(&hAlg, BCRYPT_SHA256_ALGORITHM, NULL, 0) != 0)
    {
        std::cerr << "Failed to open algorithm provider.\n";
        return 1;
    }

    if (BCryptGetProperty(hAlg, BCRYPT_OBJECT_LENGTH, (PUCHAR)&hashObjectSize, sizeof(DWORD), &cbData, 0) != 0 ||
        BCryptGetProperty(hAlg, BCRYPT_HASH_LENGTH, (PUCHAR)&cbHash, sizeof(DWORD), &cbData, 0) != 0)
    {
        std::cerr << "Failed to get hash properties.\n";
        BCryptCloseAlgorithmProvider(hAlg, 0);
        return 1;
    }

    pbHashObject = new UCHAR[hashObjectSize];
    hash.resize(cbHash);

    if (BCryptCreateHash(hAlg, &hHash, pbHashObject, hashObjectSize, NULL, 0, 0) != 0 ||
        BCryptHashData(hHash, (PUCHAR)data, strlen(data), 0) != 0 ||
        BCryptFinishHash(hHash, hash.data(), cbHash, 0) != 0)
    {
        std::cerr << "Hashing failed.\n";
        delete[] pbHashObject;
        BCryptCloseAlgorithmProvider(hAlg, 0);
        return 1;
    }

    for (BYTE b : hash)
        printf("%02x", b);
    printf("\n");

    // Cleanup
    BCryptDestroyHash(hHash);
    BCryptCloseAlgorithmProvider(hAlg, 0);
    delete[] pbHashObject;

    return 0;
}
