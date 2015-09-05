find_package(Boost REQUIRED system filesystem)
include_directories(${Boost_INCLUDE_DIRS})
add_library(dupfiles STATIC dupfiles.cpp mmap.cpp sha512/sha512.cpp)
target_link_libraries(dupfiles ${Boost_LIBRARIES})
