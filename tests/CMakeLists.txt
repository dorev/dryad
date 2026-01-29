project(tests)

add_executable(
    ${PROJECT_NAME}
    main.cpp
    tests.cpp
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    dryad
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})

set(GTEST_ROOT ${EXTERNAL_DIR}/googletest)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE ..
    PRIVATE ${GTEST_ROOT}/googletest
    PRIVATE ${GTEST_ROOT}/googletest/include
    PRIVATE ${GTEST_ROOT}/googlemock
    PRIVATE ${GTEST_ROOT}/googlemock/include
)
