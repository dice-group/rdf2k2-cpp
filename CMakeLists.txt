cmake_minimum_required(VERSION 3.16.3)
project(rdf2k2_cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wextra -g -O0")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -fomit-frame-pointer -momit-leaf-frame-pointer")
else ()
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -fomit-frame-pointer")
endif ()

find_package( Threads )
include_directories(libhdt/include/)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "libk2/src/*.cpp")
add_executable(rdf2k2_cpp "${PROJECT_SOURCE_DIR}/libk2/src/main/Main.cpp" ${SOURCES} libk2/src/index/RDFCallbackIndex.cpp libk2/src/index/RDFCallbackIndex.h libk2/src/util/LabledMatrix.cpp libk2/src/util/LabledMatrix.h libk2/src/util/Point.cpp libk2/src/util/Point.h libk2/src/util/Triple.cpp libk2/src/util/Triple.h libk2/src/util/DictEntryTriple.cpp libk2/src/util/DictEntryTriple.h libk2/src/dict/PlainDictionaryPlus.cpp libk2/src/dict/PlainDictionaryPlus.h)

target_link_libraries(rdf2k2_cpp
        ${PROJECT_SOURCE_DIR}/libhdt/lib/libhdt.so.0
        ${PROJECT_SOURCE_DIR}/libhdt/lib/libcds.so.0
        Threads::Threads)
