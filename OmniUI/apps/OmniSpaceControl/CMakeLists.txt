cmake_minimum_required(VERSION 3.16)

project(OmniSpaceControl LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(BobUI6 REQUIRED COMPONENTS Core Gui Widgets Qml Quick)

include_directories(../../core/include)
include_directories(../../deps/juce)

add_executable(OmniSpaceControl
    src/main.cpp
    ../../core/src/OmniApp.cpp
    ../../core/src/JuceWidget.cpp
    ../../core/src/space/OmniStarMap.cpp
    ../../core/src/space/OmniPropulsion.cpp
    ../../core/src/space/OmniTelemetry.cpp
    ../../core/src/space/OmniOrbitalMechanics.cpp
    ../../core/src/widgets/OmniButton.cpp
    ../../core/src/widgets/OmniDial.cpp
    ../../core/src/charts/OmniLineChart.cpp
    ../../core/src/system/OmniSystem.cpp
    ../../deps/juce/juce_mock.cpp
)

target_link_libraries(OmniSpaceControl PRIVATE
    BobUI6::Core BobUI6::Gui BobUI6::Widgets BobUI6::Qml BobUI6::Quick
)
