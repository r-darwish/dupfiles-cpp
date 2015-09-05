#pragma once

#include <boost/filesystem.hpp>

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
};

}
