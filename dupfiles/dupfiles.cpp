#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <dupfiles.hpp>
#include <boost/filesystem.hpp>
#include <boost/cstdint.hpp>
#include <boost/crc.hpp>
#include "mmap.hpp"

namespace dupfiles {

static unsigned long hash_file(const boost::filesystem::directory_entry & entry)
{
    boost::crc_32_type result;
    MemoryMap map(entry);
    result.process_bytes(map.map(), map.size());
    return static_cast<unsigned long>(result.checksum());
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
            unsigned long,
            std::vector<boost::filesystem::directory_entry>>> map;

    boost::filesystem::recursive_directory_iterator iter(path);
    std::vector<std::string> group;
    for (const auto entry : iter) {
        if (boost::filesystem::is_directory(entry)) {
            continue;
        }

        auto size = boost::filesystem::file_size(entry);
        auto entry_hash = size == 0 ? 0 : hash_file(entry);
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
