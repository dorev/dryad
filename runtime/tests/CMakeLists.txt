project(tests)

set(GTEST_ROOT ${CMAKE_SOURCE_DIR}/external/googletest)

file(GLOB_RECURSE
    LIB_SOURCES
)

add_executable(
    ${PROJECT_NAME}
    main.cpp
    tests.cpp
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC ${GTEST_ROOT}/googletest
    PUBLIC ${GTEST_ROOT}/googletest/include
    PUBLIC ${GTEST_ROOT}/googlemock
    PUBLIC ${GTEST_ROOT}/googlemock/include
)
