#pragma once

#include <string.h>
#include "sha512/sha512.hpp"

namespace dupfiles {

class HashDigest
{
public:
    HashDigest() {
        memset(block_m, 0, sizeof(block_m));
    };

    HashDigest(SHA512 & sha512) {
        sha512.final(block_m);
    }

    static const unsigned int DIGEST_SIZE = SHA512::DIGEST_SIZE;

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
