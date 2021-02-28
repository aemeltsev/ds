cmake_minimum_required(VERSION 3.5)

project(vector LANGUAGES CXX)
include_directories(inc src)

add_library(scl_vector STATIC inc/vector.h src/vector.cpp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_executable(vector main.cpp)
