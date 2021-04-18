cmake_minimum_required(version 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)Î

project(dryad)

file(GLOB_RECURSE SRC *.cpp *.h)

add_library(dryad static SRC)
