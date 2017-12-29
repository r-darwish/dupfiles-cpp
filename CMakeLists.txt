cmake_minimum_required(VERSION 3.0)

project(Dupfiles)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    set(COMPILE_OPTIONS /MT)
else()
    set(COMPILE_OPTIONS
      -pipe
      -fdiagnostics-color=always
      -Wall
      -Wextra
      -Werror
      -Wconversion)
endif()

add_compile_options(${COMPILE_OPTIONS})
include_directories(${CMAKE_SOURCE_DIR}/include)
if(("${CMAKE_SYSTEM}" MATCHES "Linux") OR (MINGW))
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
endif()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_subdirectory(src/lib)
add_subdirectory(src/cli)

if("${CMAKE_SYSTEM}" MATCHES "Linux")
    set_target_properties(finddup PROPERTIES LINK_SEARCH_END_STATIC 1)
endif()
