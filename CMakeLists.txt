cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(dryadtests)

add_executable(dryadtest tests/main.cpp tests/model_tests.cpp)

target_include_directories(dryadtest PRIVATE "${CMAKE_CURRENT_LIST_DIR}")
target_include_directories(dryadtest PRIVATE "${CMAKE_CURRENT_LIST_DIR}/extern")
target_include_directories(dryadtest PRIVATE "${CMAKE_CURRENT_LIST_DIR}/extern/gtest")
target_include_directories(dryadtest PRIVATE "${CMAKE_CURRENT_LIST_DIR}/extern/gtest/include")

target_link_libraries(dryadtest core)

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/dryad/core")

add_custom_target(run_test 
    ALL
    DEPENDS dryadtest
    COMMAND $<TARGET_FILE:dryadtest>
)