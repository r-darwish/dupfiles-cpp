#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <dupfiles.hpp>
#ifdef _MSC_VER
#include <iso646.h>
#endif
#include <boost/filesystem.hpp>
#include <boost/cstdint.hpp>
#include <boost/iostreams/device/mapped_file.hpp>
#include "file_hash.hpp"

namespace dupfiles {

std::vector<std::vector<std::string>> findDuplicates(const std::string & path, const ErrorCallback & error_callback)
{
    if (not boost::filesystem::is_directory(path)) {
        throw NotADirectory();
    }

    std::vector<std::vector<std::string>> result;
    std::unordered_map<
        FileHash,
        std::vector<boost::filesystem::directory_entry>> map;

    boost::filesystem::recursive_directory_iterator iter(path);
    std::vector<std::string> group;
    for (const auto entry : iter) {
        if (boost::filesystem::is_directory(entry)) {
            continue;
        }

    	try {
            auto entry_hash = hash_file(entry);
            map[entry_hash].push_back(std::move(entry));
        } catch (const std::exception & e) {
            auto message = entry.path().string() + ": " + e.what();
            error_callback(message);
            continue;
        }
    }

    for (const auto map_iter : map) {
        if (map_iter.second.size() <= 1) {
            continue;
        }

        std::vector<std::string> paths;
        paths.reserve(map_iter.second.size());
        for (auto entry : map_iter.second) {
            paths.push_back(entry.path().string());
        }
        result.push_back(paths);
    }

    return result;
}

}
