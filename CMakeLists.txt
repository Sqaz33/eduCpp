cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project(edu)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(eduCpp main.cpp)
