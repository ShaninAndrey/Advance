cmake_minimum_required(VERSION 3.7)
project(Voronoi)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        Console_Example.cpp
        all.hpp)

add_executable(Voronoi ${SOURCE_FILES})