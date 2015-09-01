#include <fcntl.h>
#include <boost/filesystem.hpp>
#include <sys/mman.h>
#include <sstream>
#include "mmap.hpp"

namespace dupfiles {

class OpenError: public std::exception
{
public:
    OpenError(const std::string & path, int err) :
        std::exception(),
        path_(path),
        err_(err)
    {
        std::ostringstream stream;
        stream << "Cannot open " << path_ << ": " << err_;
        what_.assign(stream.str());
    }

    virtual const char* what() const throw()
    {
        return what_.c_str();
    }

private:
    std::string what_;
    std::string path_;
    int err_;
};

MemoryMap::MemoryMap(const boost::filesystem::directory_entry & entry):
    size_(boost::filesystem::file_size(entry)) {
    auto fd = open(entry.path().string().c_str(), O_RDONLY);
    if (fd == -1) {
        throw OpenError(entry.path().string(), errno);
    }

    auto map_ = mmap(nullptr, size_, PROT_READ, MAP_SHARED, fd, 0);
    close(fd);
    if (map_ == MAP_FAILED) {
        throw OpenError(entry.path().string(), errno);
    }

    this->map_ = map_;
}

MemoryMap::~MemoryMap()
{
    munmap(map_, size_);
}

}
