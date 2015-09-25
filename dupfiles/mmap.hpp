#pragma once

#include <boost/filesystem.hpp>
#ifdef _WIN32
#include <windows.h>
#endif

namespace dupfiles {

class MemoryMap
{
public:
    MemoryMap(const boost::filesystem::directory_entry & entry);
    ~MemoryMap();
    void close();
    const void * map() { return map_; }
    std::size_t size() { return size_; }

private:
    void * map_;
    std::size_t size_;
#ifdef _WIN32
    HANDLE map_handle_ = INVALID_HANDLE_VALUE;
#endif
};

}
