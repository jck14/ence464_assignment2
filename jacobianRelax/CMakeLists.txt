cmake_minimum_required(VERSION 3.8)
project(jacobianRelax)
include_directories(/usr/local/lib)
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)
add_executable(jacobianRelax ${SOURCE_FILES})