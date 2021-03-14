cmake_minimum_required(VERSION 3.5)

project(array LANGUAGES CXX)
include_directories(inc src)

add_library(scl_array STATIC inc/array.h src/array.cpp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_executable(array main.cpp)
