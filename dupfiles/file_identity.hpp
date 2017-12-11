#pragma once

#include <openssl/sha.h>
#include <string.h>
#include <array>
#include <boost/functional/hash.hpp>
#include <boost/filesystem.hpp>
#include <boost/iostreams/device/mapped_file.hpp>

namespace dupfiles {

using HashDigest = std::array<unsigned char, SHA_DIGEST_LENGTH>;
using FileIdentity = std::pair<boost::uintmax_t, HashDigest>;

inline FileIdentity get_file_identity(const boost::filesystem::directory_entry & entry)
{
    if (boost::filesystem::file_size(entry) == 0) {
        return FileIdentity(0, HashDigest());
    }

    boost::iostreams::mapped_file_source map(entry.path());
    HashDigest digest;
    SHA1(reinterpret_cast<const unsigned char *>(map.data()), map.size(), digest.data());
    map.close();
    return FileIdentity(map.size(), digest);
}

}

namespace std {

template <> struct hash<dupfiles::FileIdentity> {
    size_t operator()(const dupfiles::FileIdentity &x) const
    {
        std::size_t seed = x.first;
        boost::hash_combine(seed, boost::hash<decltype(x.second)>()(x.second));
        return seed;
    }
};

}
