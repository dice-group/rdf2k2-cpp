cmake_minimum_required(VERSION 3.17)
project(rdf2k2_cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

include_directories(libhdt/include/)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "libk2/src/*.cpp")
add_executable(rdf2k2_cpp "${PROJECT_SOURCE_DIR}/libk2/src/main/Main.cpp" ${SOURCES} libk2/src/index/RDFCallbackIndex.cpp libk2/src/index/RDFCallbackIndex.h libk2/src/util/LabledMatrix.cpp libk2/src/util/LabledMatrix.h libk2/src/util/Point.cpp libk2/src/util/Point.h libk2/src/util/Triple.cpp libk2/src/util/Triple.h)

#find_library(LIBHDT hdt libhdt/lib/)
target_link_libraries(rdf2k2_cpp ${PROJECT_SOURCE_DIR}/libhdt/lib/libhdt.so.0)
target_link_libraries(rdf2k2_cpp ${PROJECT_SOURCE_DIR}/libhdt/lib/libcds.so.0)
