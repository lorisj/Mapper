cmake_minimum_required(VERSION 3.15.3)
project(mapper)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-Wall -Werror -pedantic --std=c++11 -g")

add_executable(mapper.exe mapper.cpp mapper.h)
add_executable(test.exe test.cpp)

