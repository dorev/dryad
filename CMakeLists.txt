cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(MAKE_DIRECTORY ./test)

project(dryad_tests)

# GoogleTest
target_include_directories(dryad_tests PRIVATE ./external/gtest/googletest/include)
target_link_libraries(dryad_test ./external/gtest/buildgtest/googletest/include)
target_link_libraries(dryad_test ./external/gtest/build/lib/Release/gtest.lib)

