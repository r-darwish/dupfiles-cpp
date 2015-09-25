#include <windows.h>
#include <boost/filesystem.hpp>
#include <sstream>
#include "mmap.hpp"

namespace dupfiles {

class SyscallError: public std::exception
{
public:
    SyscallError(const char * syscall, const std::string & path, DWORD err) :
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
    DWORD err_;
};

MemoryMap::MemoryMap(const boost::filesystem::directory_entry & entry):
    size_(boost::filesystem::file_size(entry)) {
    auto file_handle = CreateFile(entry.path().string().c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file_handle == INVALID_HANDLE_VALUE) {
        throw SyscallError("CreateFile", entry.path().string(), GetLastError());
    }
    map_handle_ = CreateFileMapping(file_handle, NULL, PAGE_READONLY, 0, 0, NULL);
    CloseHandle(file_handle);
    if (map_handle_ == INVALID_HANDLE_VALUE) {
        throw SyscallError("CreateFileMapping", entry.path().string(), GetLastError());
    }

    map_ = MapViewOfFile(map_handle_, FILE_MAP_READ, 0, 0, 0);
    if (map_ == nullptr) {
        close();
        throw SyscallError("MapViewOfFile", entry.path().string(), GetLastError());
    }

}

void MemoryMap::close() {
    if (map_ != nullptr) {
        UnmapViewOfFile(map_);
        map_ = nullptr;
    }

    if (map_handle_ != INVALID_HANDLE_VALUE) {
        CloseHandle(map_handle_);
        map_handle_ = INVALID_HANDLE_VALUE;
    }
}

MemoryMap::~MemoryMap()
{
    try {
        close();
    } catch (SyscallError) {

    }
}

}
