cmake_minimum_required(VERSION 3.0)

include("HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.12.45.tar.gz"
    SHA1 "a1d363c7ffb324eb55a410715868ada32e595622"
)

project(Dupfiles)

if(MSVC)
    set(COMPILE_OPTIONS /MT)
else()
    set(COMPILE_OPTIONS
      -pipe
      -fdiagnostics-color=always
      -Wall
      -Wextra
      -Werror
      -Wconversion
      -std=c++11
      -O3
    )
endif()

add_compile_options(${COMPILE_OPTIONS})
include_directories(${CMAKE_SOURCE_DIR}/include)
if(("${CMAKE_SYSTEM}" MATCHES "Linux") OR (MINGW))
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
endif()

set(Boost_USE_STATIC_LIBS ON)
if(WIN32)
    set(CMAKE_FIND_LIBRARY_SUFFIXES .lib .a)
else()
    set(CMAKE_FIND_LIBRARY_SUFFIXES .a)
endif()

add_subdirectory(dupfiles)
add_subdirectory(finddup)

if("${CMAKE_SYSTEM}" MATCHES "Linux")
    set_target_properties(finddup PROPERTIES LINK_SEARCH_END_STATIC 1)
endif()
