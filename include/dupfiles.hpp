#pragma once

#include <functional>
#include <string>
#include <vector>

namespace dupfiles {

class NotADirectory : public std::exception {
    virtual const char * what() const throw() { return "The given path is not a directory"; }
};

using ErrorCallback = std::function<void(std::string)>;

std::vector<std::vector<std::string>> find_duplicates(const std::string & path,
                                                      const ErrorCallback & error_callback);

} // namespace dupfiles
