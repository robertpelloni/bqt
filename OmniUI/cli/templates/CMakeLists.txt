cmake_minimum_required(VERSION 3.16)

project(HelloOmni LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

<<<<<<< HEAD
<<<<<<< HEAD
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Qml Quick)
=======
>>>>>>> feature/omni-ui-framework-18001284211800334382
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Qml Quick SerialPort Bluetooth)
=======
find_package(BobUI6 REQUIRED COMPONENTS Core Gui Widgets Qml Quick)
find_package(BobUI6 REQUIRED COMPONENTS Core Gui Widgets Qml Quick SerialPort Bluetooth)
>>>>>>> origin/dev

include_directories(../../core/include)
# We point to the local deps folder for the mock JUCE headers
include_directories(../../deps/juce)

add_executable(HelloOmni
    src/main.cpp
    ../../core/src/OmniApp.cpp
    ../../core/src/JuceWidget.cpp
    ../../deps/juce/juce_mock.cpp
)

target_link_libraries(HelloOmni PRIVATE
    BobUI6::Core BobUI6::Gui BobUI6::Widgets BobUI6::Qml BobUI6::Quick
    BobUI6::SerialPort BobUI6::Bluetooth
)

if(EMSCRIPTEN)
    target_link_options(HelloOmni PRIVATE -s USE_PTHREADS=0)
endif()
