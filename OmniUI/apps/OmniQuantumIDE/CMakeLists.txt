cmake_minimum_required(VERSION 3.16)

project(OmniQuantumIDE LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Qml Quick)

include_directories(../../core/include)
include_directories(../../deps/juce)

add_executable(OmniQuantumIDE
    src/main.cpp
    ../../core/src/OmniApp.cpp
    ../../core/src/JuceWidget.cpp
    ../../core/src/quantum/OmniQubit.cpp
    ../../core/src/quantum/OmniQuantumGate.cpp
    ../../core/src/quantum/OmniQuantumCircuit.cpp
    ../../core/src/quantum/OmniQuantumSimulator.cpp
    ../../core/src/3d/OmniView3D.cpp
    ../../core/src/3d/OmniMesh.cpp
    ../../core/src/3d/OmniCamera.cpp
    ../../core/src/widgets/OmniButton.cpp
    ../../core/src/system/OmniSystem.cpp
    ../../deps/juce/juce_mock.cpp
)

target_link_libraries(OmniQuantumIDE PRIVATE
    Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Qml Qt6::Quick
)
