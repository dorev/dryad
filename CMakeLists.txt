cmake_minimum_required(VERSION 3.6)

# set the project name
project(dryad)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(DryadTest tests/tests.cpp)