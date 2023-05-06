cmake_minimum_required(VERSION 3.5)

set(BIN sm_ptr)
project(${BIN} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include_directories(inc src tests)

############################################################
# Searches libraries and check it`s
############################################################
find_package(Threads)

if(Threads_FOUND)
    message ("Thread library was found")
else()
    message (FATAL_ERROR "Cannot find Threads package")
endif()

find_package(GTest)

if(GTest_FOUND)
    message("Google Test library was found")
else()
    message(FATAL_ERROR "Cannot find Google Test package")
endif()

add_compile_options(-O2)
link_libraries(pthread)
link_libraries(gtest)

############################################################
# Create a library
############################################################
add_library(pl_sm_ptr STATIC inc/sm_ptr.h src/sm_ptr.cpp)

############################################################
# Create an executable
############################################################
set(HEADERS sm_ptr.h tests.h)
set(SOURCES main.cpp)
set(REQUIRED_LIBRARIES pthread gtest)

add_compile_options(-std=c++11 -g -Wall)

add_executable(sm_ptr main.cpp)
target_link_libraries(${BIN} PRIVATE ${EXTRA_LIBS} ${REQUIRED_LIBRARIES})
############################################################
# Install
############################################################
