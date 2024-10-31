project(dryad)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror" PARENT_SCOPE)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX" PARENT_SCOPE)
endif()

file(GLOB_RECURSE
    SOURCES
    src/*.h
    src/*.cpp
)

add_library(
    ${PROJECT_NAME}
    ${SOURCES}
)
