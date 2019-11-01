cmake_minimum_required(VERSION 3.7.2)
project(cppmutexexperiment)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads)
find_package(benchmark) # Uses google benchmark package it needs to be compiled on your machine

# Always compile in release
set(CMAKE_BUILD_TYPE RelWithDebInfo)
#set(CMAKE_BUILD_TYPE Release)

# Warnings to errors
#add_definitions(-Wall -Werror -Wdeprecated-declarations)
# -Wdeprecated-declarations added to suppress asio deprecation error
add_definitions(-Wall -Werror -Wdeprecated-declarations)

file(GLOB SOURCES "*.cpp")

add_executable(cppmutexexperiment ${SOURCES})
target_link_libraries(cppmutexexperiment
  benchmark
  ${CMAKE_THREAD_LIBS_INIT})
