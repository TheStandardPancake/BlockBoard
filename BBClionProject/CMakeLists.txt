cmake_minimum_required(VERSION 3.22)
project(BBClionProject)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
link_libraries(ws2_32 wsock32)

add_executable(BlockBoard BlockBoard.cpp)
