project(gui)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

set(IMGUI_DIR ${EXTERNAL_DIR}/imgui)

file(GLOB_RECURSE
    GUI_SOURCES
    main.cpp
    ${IMGUI_DIR}/*.h
    ${IMGUI_DIR}/*.cpp
)

add_executable(${PROJECT_NAME}
    ${GUI_SOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ./
    ${IMGUI_DIR}
)

target_link_libraries(${PROJECT_NAME}
    OpenGL::GL
    SDL2::SDL2
    dryad
)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

install(DIRECTORY ${SDL_LIB_DIR}/
    DESTINATION bin
    FILES_MATCHING PATTERN ${PLATFORM_LIB_EXTENSION}
)
