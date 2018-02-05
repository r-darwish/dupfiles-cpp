#include <dupfiles.hpp>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#ifdef _MSC_VER
#include <iso646.h>
#endif
#include "file_identity.hpp"
#include <boost/cstdint.hpp>
#include <boost/filesystem.hpp>
#include <boost/iostreams/device/mapped_file.hpp>

namespace dupfiles {

using IdentityMap = std::unordered_map<FileIdentity, std::vector<boost::filesystem::directory_entry>>;

inline std::vector<std::vector<std::string>> prepare_results(const IdentityMap & map)
{
    std::vector<std::vector<std::string>> result;

    for (const auto& map_iter : map) {
        const auto & files = map_iter.second;
        if (files.size() <= 1) {
            continue;
        }

        std::vector<std::string> paths;
        paths.reserve(files.size());
        for (auto entry : files) {
            paths.emplace_back(entry.path().string());
        }
        result.emplace_back(std::move(paths));
    }

    return result;
}

std::vector<std::vector<std::string>> find_duplicates(const std::string & path,
                                                      const ErrorCallback & error_callback)
{
    if (not boost::filesystem::is_directory(path)) {
        throw NotADirectory();
    }

    IdentityMap map;

    boost::filesystem::recursive_directory_iterator iter(path);
    for (const auto& entry : iter) {
        if (boost::filesystem::is_directory(entry)) {
            continue;
        }

        try {
            auto entry_hash = get_file_identity(entry);
            map[entry_hash].emplace_back(std::move(entry));
        } catch (const std::exception & e) {
            auto message = entry.path().string() + ": " + e.what();
            error_callback(std::move(message));
            continue;
        }
    }

    return prepare_results(map);
}

} // namespace dupfiles
