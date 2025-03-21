cmake_minimum_required(VERSION 3.5)

project(algroutmy)

set(CMAKE_CXX_STANDARD 23)

SET(GCC_COVERAGE_COMPILE_FLAGS "-mavx2")

add_executable(algroutmy main.cpp)

install(TARGETS algroutmy RUNTIME DESTINATION bin)
