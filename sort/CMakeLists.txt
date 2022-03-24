cmake_minimum_required(VERSION 3.21)
project(private_sort)

set(CMAKE_CXX_STANDARD 14)

add_executable(private_sort main.cpp sort.h doctest.h)
