cmake_minimum_required(VERSION 3.10)

# Compilation settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Variables
set(ROOT "${CMAKE_CURRENT_LIST_DIR}")
set(TMP "${ROOT}/tmp")
set(RELEASE_BINAIRIES "${TMP}/release/bin")
set(DEBUG_BINAIRIES "${TMP}/debug/bin")

# CMake variables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${RELEASE_BINARIES}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${DEBUG_BINARIES}")

# Dryad project
# =============================================================================
project(dryad)
file(GLOB_RECURSE sources "${ROOT}/src/*.cpp")
file(GLOB_RECURSE includes "${ROOT}/includes/*.h")
add_library(dryad STATIC ${sources} ${includes})

target_include_directories(dryad PRIVATE "${ROOT}/extern")

# Tests project
# =============================================================================
#project(dryadtests)
#add_executable(dryadtest "${ROOT}/tests/*.cpp")
#target_link_libraries(dryadtest PUBLIC dryad)
#target_include_directories(dryadtests PRIVATE "${ROOT}/extern")
#target_include_directories(dryadtests PRIVATE "${ROOT}/includes")