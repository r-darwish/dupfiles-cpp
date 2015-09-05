#include <fcntl.h>
#include <boost/filesystem.hpp>
#include <sys/mman.h>
#include <sstream>
#include "mmap.hpp"

namespace dupfiles {

class SyscallError: public std::exception
{
public:
    SyscallError(const char * syscall, const std::string & path, int err) :
        std::exception(),
        syscall_(syscall),
        path_(path),
        err_(err)
    {
        std::ostringstream stream;
        stream << syscall_ << " failed " << path_ << ": " << err_;
        what_.assign(stream.str());
    }

    virtual const char* what() const throw()
    {
        return what_.c_str();
    }

private:
    const char * syscall_;
    std::string what_;
    std::string path_;
    int err_;
};

MemoryMap::MemoryMap(const boost::filesystem::directory_entry & entry):
    size_(boost::filesystem::file_size(entry)) {
    auto fd = open(entry.path().string().c_str(), O_RDONLY);
    if (fd == -1) {
        throw SyscallError("open", entry.path().string(), errno);
    }

    auto map_ = mmap(nullptr, size_, PROT_READ, MAP_SHARED, fd, 0);
    ::close(fd);
    if (map_ == MAP_FAILED) {
        throw SyscallError("mmap", entry.path().string(), errno);
    }

    this->map_ = map_;
}

void MemoryMap::close() {
    if (map_ == nullptr) {
        return;
    }

    if (munmap(map_, size_) != 0) {
        throw SyscallError("munmap", "", errno);
    }

    map_ = nullptr;
}

MemoryMap::~MemoryMap()
{
    try {
        close();
    } catch (SyscallError) {

    }
}

}
