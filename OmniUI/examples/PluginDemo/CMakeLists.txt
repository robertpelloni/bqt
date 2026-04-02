cmake_minimum_required(VERSION 3.16)

project(PluginDemo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Qml Quick)

include_directories(../../omnicore/include)
# We point to the local deps folder for the mock JUCE headers
include_directories(../../deps/juce)

add_executable(PluginDemo
    src/main.cpp
    ../../omnicore/src/OmniApp.cpp
    ../../omnicore/src/OmniNativeEventFilter.cpp
    ../../omnicore/src/OmniQmlRegistration.cpp
    ../../omnicore/src/JuceWidget.cpp
    ../../omnicore/src/widgets/OmniButton.cpp
    ../../omnicore/src/widgets/OmniSlider.cpp
    ../../omnicore/src/widgets/OmniTextField.cpp
    ../../omnicore/src/widgets/OmniDial.cpp
    ../../omnicore/src/widgets/OmniCheckBox.cpp
    ../../omnicore/src/widgets/OmniComboBox.cpp
    ../../omnicore/src/widgets/OmniProgressBar.cpp
    ../../omnicore/src/charts/OmniLineChart.cpp
    ../../omnicore/src/charts/OmniOscilloscope.cpp
    ../../omnicore/src/layout/OmniDockArea.cpp
    ../../omnicore/src/layout/OmniSplitView.cpp
    ../../omnicore/src/net/OmniHttpClient.cpp
    ../../omnicore/src/net/OmniWebSocket.cpp
    ../../omnicore/src/audio/dsp/OmniGain.cpp
    ../../omnicore/src/audio/dsp/OmniFilter.cpp
    ../../omnicore/src/plugin/OmniPluginManager.cpp
    ../../omnicore/src/data/OmniDatabase.cpp
    ../../omnicore/src/gfx/OmniShaderEffect.cpp
    ../../omnicore/src/gfx/OmniParticles.cpp
    ../../omnicore/src/widgets/OmniCodeEditor.cpp
    ../../omnicore/src/widgets/OmniCalendar.cpp
    ../../deps/juce/juce_mock.cpp
)

target_link_libraries(PluginDemo PRIVATE
    Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Qml Qt6::Quick
)

if(EMSCRIPTEN)
    target_link_options(HelloOmni PRIVATE -s USE_PTHREADS=0)
endif()
