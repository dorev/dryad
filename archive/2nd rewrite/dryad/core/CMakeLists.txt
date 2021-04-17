project(core)

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)

add_library(core STATIC "${SOURCES}")

target_include_directories(core PRIVATE "${CMAKE_CURRENT_LIST_DIR}/../../extern")
target_include_directories(core PRIVATE "${CMAKE_CURRENT_LIST_DIR}/../../dryad")
