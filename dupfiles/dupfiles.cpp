#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <dupfiles.hpp>
#include <boost/filesystem.hpp>
#include <boost/cstdint.hpp>
#include "sha512/sha512.hpp"
#include "hash_digest.hpp"
#include "mmap.hpp"

namespace dupfiles {

static HashDigest hash_file(const boost::filesystem::directory_entry & entry)
{
    MemoryMap map(entry);
    SHA512 sha512;
    sha512.init();
    sha512.update(static_cast<const unsigned char *>(map.map()), map.size());
    return HashDigest(sha512);
}

std::vector<std::vector<std::string>> findDuplicates(const std::string & path)
{
    if (not boost::filesystem::is_directory(path)) {
        throw NotADirectory();
    }

    std::vector<std::vector<std::string>> result;
    std::unordered_map<
        boost::uintmax_t,
        std::unordered_map<
            HashDigest,
            std::vector<boost::filesystem::directory_entry>>> map;

    boost::filesystem::recursive_directory_iterator iter(path);
    std::vector<std::string> group;
    for (const auto entry : iter) {
        if (boost::filesystem::is_directory(entry)) {
            continue;
        }

        auto size = boost::filesystem::file_size(entry);
        auto entry_hash = size == 0 ? HashDigest() : hash_file(entry);
        map[size][entry_hash].push_back(std::move(entry));
    }

    for (const auto iter1 : map) {
        for (const auto iter2 : iter1.second) {
            std::vector<std::string> paths;
            if (iter2.second.size() > 1) {
                for (const auto path_iter : iter2.second) {
                    paths.push_back(std::move(path_iter.path().string()));
                }
                result.push_back(std::move(paths));
            }
        }
    }

    return result;
}

}
