hunter_add_package(Boost COMPONENTS system filesystem iostreams)
hunter_add_package(OpenSSL)

find_package(Boost REQUIRED system filesystem iostreams)
find_package(OpenSSL)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(BEFORE SYSTEM ${OPENSSL_INCLUDE_DIR})
add_library(dupfiles STATIC dupfiles.cpp)
target_link_libraries(dupfiles ${Boost_LIBRARIES} ${OPENSSL_CRYPTO_LIBRARY})
