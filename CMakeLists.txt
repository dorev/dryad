cmake_minimum_required(VERSION 3.14)

project(DryadSolution)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)

add_subdirectory(dryad)
add_subdirectory(tests)
#add_subdirectory(gui)
