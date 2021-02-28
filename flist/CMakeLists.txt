cmake_minimum_required(VERSION 3.5)

project(flist LANGUAGES CXX)
include_directories(inc src)

add_library(scl_flist STATIC inc/flist.h src/flist.cpp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(flist main.cpp)

