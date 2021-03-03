cmake_minimum_required(VERSION 3.5)

project(deque LANGUAGES CXX)
include_directories(inc src)

add_library(scl_deque STATIC inc/deque.h src/deque.cpp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_executable(deque main.cpp)
