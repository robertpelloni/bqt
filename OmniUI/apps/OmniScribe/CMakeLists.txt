cmake_minimum_required(VERSION 3.16)

project(OmniScribe LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Qml Quick)

include_directories(../../core/include)
include_directories(../../deps/juce)

add_executable(OmniScribe
    src/main.cpp
    ../../core/src/OmniApp.cpp
    ../../core/src/JuceWidget.cpp
    ../../core/src/language/OmniTokenizer.cpp
    ../../core/src/language/OmniTransformer.cpp
    ../../core/src/language/OmniContextWindow.cpp
    ../../core/src/language/OmniLinguisticModel.cpp
    ../../core/src/widgets/OmniButton.cpp
    ../../core/src/widgets/OmniCodeEditor.cpp
    ../../core/src/widgets/highlight/OmniSyntaxHighlighter.cpp
    ../../core/src/system/OmniSystem.cpp
    ../../deps/juce/juce_mock.cpp
)

target_link_libraries(OmniScribe PRIVATE
    Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Qml Qt6::Quick
)
