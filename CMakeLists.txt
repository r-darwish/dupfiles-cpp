cmake_minimum_required(VERSION 3.0)
project(dupfiles)
set(COMPILE_OPTIONS
  -pipe
  -fdiagnostics-color=always
  -Wall
  -Wextra
  -Werror
  -std=c++11
  -stdlib=libc++
)

add_compile_options(${COMPILE_OPTIONS})
foreach(COMPILE_OPTION IN LISTS COMPILE_OPTIONS)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${COMPILE_OPTION}")
endforeach(COMPILE_OPTION)
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
include_directories(${CMAKE_SOURCE_DIR}/include)

set(Boost_USE_STATIC_LIBS ON)
add_subdirectory(dupfiles)
add_subdirectory(finddup)
