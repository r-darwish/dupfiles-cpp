cmake_minimum_required(VERSION 3.0)
project(dupfiles)
set(COMPILE_OPTIONS
  -pipe
  -fdiagnostics-color=always
  -Wall
  -Wextra
  -Werror
  -Wconversion
  -std=c++11
)

add_compile_options(${COMPILE_OPTIONS})
foreach(COMPILE_OPTION IN LISTS COMPILE_OPTIONS)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${COMPILE_OPTION}")
endforeach(COMPILE_OPTION)
include_directories(${CMAKE_SOURCE_DIR}/include)
if(("${CMAKE_SYSTEM}" MATCHES "Linux") OR (MINGW))
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
endif()

set(Boost_USE_STATIC_LIBS ON)
add_subdirectory(dupfiles)
add_subdirectory(finddup)

if("${CMAKE_SYSTEM}" MATCHES "Linux")
    set_target_properties(finddup PROPERTIES LINK_SEARCH_END_STATIC 1)
endif()
