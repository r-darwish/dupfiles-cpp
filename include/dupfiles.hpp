#pragma once

#include <string>
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


std::vector<std::vector<std::string>> findDuplicates(const std::string & path);

}
