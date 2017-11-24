cmake_minimum_required(VERSION 3.0)

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

 include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_subdirectory(dupfiles)
add_subdirectory(finddup)

if("${CMAKE_SYSTEM}" MATCHES "Linux")
    set_target_properties(finddup PROPERTIES LINK_SEARCH_END_STATIC 1)
endif()
