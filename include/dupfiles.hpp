#pragma once

#include <string>
#include <functional>
#include <vector>

namespace dupfiles
{

class NotADirectory: public std::exception
{
    virtual const char* what() const throw()
    {
        return "The given path is not a directory";
    }
};

using ErrorCallback = std::function<void(const char *)>;


std::vector<std::vector<std::string>> findDuplicates(const std::string & path, const ErrorCallback & error_callback);

}
