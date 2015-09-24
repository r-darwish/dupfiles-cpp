#pragma once

#include <openssl/sha.h>
#include <string.h>

namespace dupfiles {

class HashDigest
{
public:
    HashDigest() {
        memset(block_m, 0, sizeof(block_m));
    };

    HashDigest(const void * buffer, size_t length) {
        SHA512_CTX sha;
        SHA512_Init(&sha);
        SHA512_Update(&sha, buffer, length);
        SHA512_Final(block_m, &sha);
    }

    static const unsigned int DIGEST_SIZE = SHA512_DIGEST_LENGTH;

    const unsigned char * block() const { return block_m; }

    bool operator==(const HashDigest & rhs) const{
        return memcmp(block_m, rhs.block_m, sizeof(block_m)) == 0;
    }

private:
    unsigned char block_m[DIGEST_SIZE];
};

}

namespace std {

template <> struct hash<dupfiles::HashDigest> {
    size_t operator()(const dupfiles::HashDigest &x) const
    {
        size_t h = 0;
        const unsigned char * block = x.block();
        for (unsigned int i = 0; i < dupfiles::HashDigest::DIGEST_SIZE; i++) {
            h ^= std::hash<unsigned char>()(block[i]);
        }
        return h;
    }
};

}
