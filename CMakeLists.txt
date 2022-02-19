
cmake_minimum_required(VERSION 3.21)
project(untitled)

set(CMAKE_CXX_STANDARD 14)

add_executable(untitled main.cpp list.h doctest.h manager/main.cpp manager/manager.h)
