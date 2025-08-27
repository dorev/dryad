cmake_minimum_required(VERSION 3.15)

project(DryadJuceDemo VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  JUCE
  GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
  GIT_TAG 7.0.9
)
FetchContent_MakeAvailable(JUCE)

juce_add_gui_app(DryadJuceDemo
    PRODUCT_NAME "Dryad JUCE Demo"
)

juce_generate_juce_header(DryadJuceDemo)

target_sources(DryadJuceDemo
    PRIVATE
        src/Main.cpp
        src/MainComponent.cpp
        src/MainComponent.h
        src/NodeGraphComponent.cpp
        src/NodeGraphComponent.h
        src/PianoRollComponent.cpp
        src/PianoRollComponent.h
        src/TransportComponent.cpp
        src/TransportComponent.h
)

target_compile_definitions(DryadJuceDemo
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
)

target_link_libraries(DryadJuceDemo
    PRIVATE
        juce::juce_audio_utils
        juce::juce_gui_extra
        dryad
)
