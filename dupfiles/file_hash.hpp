#pragma once

#include <openssl/sha.h>
#include <string.h>
#include <array>
#include <boost/functional/hash.hpp>

namespace dupfiles {

using HashDigest = std::array<unsigned char, SHA_DIGEST_LENGTH>;
using FileHash = std::pair<boost::uintmax_t, HashDigest>;

static FileHash hash_file(const boost::filesystem::directory_entry & entry)
{
    if (boost::filesystem::file_size(entry) == 0) {
        return FileHash(0, HashDigest());
    }

    boost::iostreams::mapped_file_source map(entry.path());
    HashDigest digest;
    SHA1(reinterpret_cast<const unsigned char *>(map.data()), map.size(), digest.data());
    map.close();
    return FileHash(map.size(), digest);
}

}

namespace std {

template <> struct hash<dupfiles::FileHash> {
    size_t operator()(const dupfiles::FileHash &x) const
    {
        std::size_t seed = x.first;
        boost::hash_combine(seed, boost::hash<decltype(x.second)>()(x.second));
        return seed;
    }
};

}
