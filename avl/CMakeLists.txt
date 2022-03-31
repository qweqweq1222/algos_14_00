cmake_minimum_required(VERSION 3.21)
project(avl)

set(CMAKE_CXX_STANDARD 14)

add_executable(avl main.cpp avltree.h mm.h)
