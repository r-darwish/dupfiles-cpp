#pragma once

#include <openssl/sha.h>
#include <string.h>
#include <array>
#include <boost/functional/hash.hpp>

namespace dupfiles {

using HashDigest = std::array<unsigned char, SHA512_DIGEST_LENGTH>;

HashDigest sha512(const void * buffer, size_t length) {
    HashDigest digest;
    SHA512(static_cast<const unsigned char *>(buffer), length, digest.data());
    return digest;
}

}

namespace std {

template <> struct hash<dupfiles::HashDigest> {
    size_t operator()(const dupfiles::HashDigest &x) const
    {
        return boost::hash<decltype(x)>()(x);
    }
};

}
