cmake_minimum_required(VERSION 3.8)
project(jacobianRelax)
include_directories(/usr/local/lib)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

set(SOURCE_FILES main.cpp)
add_executable(jacobianRelax ${SOURCE_FILES})

target_link_libraries(jacobianRelax Threads::Threads)