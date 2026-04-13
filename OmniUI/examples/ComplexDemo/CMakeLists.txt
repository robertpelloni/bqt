cmake_minimum_required(VERSION 3.16)

project(ComplexDemo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

<<<<<<< HEAD
<<<<<<< HEAD
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Qml Quick Sql Network WebSockets Concurrent Multimedia)

include_directories(../../omnicore/include)
=======
>>>>>>> feature/omni-ui-framework-18001284211800334382
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Qml Quick)
=======
find_package(BobUI6 REQUIRED COMPONENTS Core Gui Widgets Qml Quick Sql Network WebSockets Concurrent Multimedia)

include_directories(../../omnicore/include)
find_package(BobUI6 REQUIRED COMPONENTS Core Gui Widgets Qml Quick)
>>>>>>> origin/dev

include_directories(../../core/include)
# We point to the local deps folder for the mock JUCE headers
include_directories(../../deps/juce)

add_executable(ComplexDemo
    src/main.cpp
<<<<<<< HEAD
    ../../omnicore/src/OmniApp.cpp
    ../../omnicore/src/OmniNativeEventFilter.cpp
    ../../omnicore/src/OmniQmlRegistration.cpp
    ../../omnicore/src/OmniRustBridge.cpp
    ../../omnicore/src/JuceWidget.cpp
    ../../omnicore/src/widgets/OmniButton.cpp
    ../../omnicore/src/widgets/OmniSlider.cpp
    ../../omnicore/src/widgets/OmniTextField.cpp
    ../../omnicore/src/widgets/OmniTerminal.cpp
    ../../omnicore/src/widgets/OmniWindow.cpp
    ../../omnicore/src/widgets/OmniListView.cpp
    ../../omnicore/src/widgets/OmniTabBar.cpp
    ../../omnicore/src/gfx/OmniAnimator.cpp
    ../../omnicore/src/widgets/OmniImGuiCanvas.cpp
    ../../omnicore/src/OmniStyleSheet.cpp
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
    ../../omnicore/src/net/OmniIPC.cpp
    ../../omnicore/src/net/OmniMeshNode.cpp
    ../../omnicore/src/OmniUserManager.cpp
    ../../omnicore/src/net/OmniVoiceMesh.cpp
    ../../omnicore/src/widgets/OmniTreeView.cpp
    ../../omnicore/src/layout/OmniStackPane.cpp
    ../../omnicore/src/layout/OmniGridPane.cpp
    ../../omnicore/src/OmniClipboard.cpp
    ../../omnicore/src/net/OmniFileMesh.cpp
    ../../omnicore/src/OmniUndoStack.cpp
    ../../omnicore/src/layout/OmniFlowPane.cpp
    ../../omnicore/src/layout/OmniTilePane.cpp
    ../../omnicore/src/audio/dsp/OmniSpatialAudio.cpp
    ../../omnicore/src/data/OmniValueTree.cpp
    ../../omnicore/src/gfx/OmniDropShadow.cpp
    ../../omnicore/src/gfx/OmniRhiNode.cpp
    ../../omnicore/src/widgets/OmniPlot.cpp
    ../../omnicore/src/audio/dsp/OmniGain.cpp
    ../../omnicore/src/audio/dsp/OmniMasterClock.cpp
    ../../omnicore/src/audio/dsp/OmniMidiHandler.cpp
    ../../omnicore/src/audio/dsp/OmniSequencer.cpp
    ../../omnicore/src/audio/dsp/OmniAudioPlayer.cpp
    ../../omnicore/src/audio/dsp/OmniSynthesizer.cpp
    ../../omnicore/src/audio/OmniVoiceEngine.cpp
    ../../omnicore/src/audio/dsp/OmniFilter.cpp
=======
>>>>>>> feature/omni-ui-framework-18001284211800334382
    ../../core/src/OmniApp.cpp
    ../../core/src/JuceWidget.cpp
    ../../core/src/widgets/OmniButton.cpp
    ../../core/src/widgets/OmniSlider.cpp
    ../../core/src/widgets/OmniTextField.cpp
    ../../core/src/widgets/OmniDial.cpp
    ../../core/src/widgets/OmniCheckBox.cpp
    ../../core/src/widgets/OmniComboBox.cpp
    ../../core/src/widgets/OmniProgressBar.cpp
    ../../core/src/charts/OmniLineChart.cpp
    ../../core/src/charts/OmniOscilloscope.cpp
    ../../core/src/layout/OmniDockArea.cpp
    ../../core/src/layout/OmniSplitView.cpp
    ../../core/src/net/OmniHttpClient.cpp
    ../../core/src/net/OmniWebSocket.cpp
    ../../core/src/audio/dsp/OmniGain.cpp
    ../../core/src/audio/dsp/OmniFilter.cpp
    ../../core/src/audio/OmniAudioMeter.cpp
    ../../core/src/widgets/highlight/OmniSyntaxHighlighter.cpp
    ../../deps/juce/juce_mock.cpp
)

target_link_libraries(ComplexDemo PRIVATE
<<<<<<< HEAD
<<<<<<< HEAD
    Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Qml Qt6::Quick Qt6::Sql Qt6::Network Qt6::WebSockets Qt6::Concurrent Qt6::Multimedia
=======
    Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Qml Qt6::Quick
>>>>>>> feature/omni-ui-framework-18001284211800334382
=======
    BobUI6::Core BobUI6::Gui BobUI6::Widgets BobUI6::Qml BobUI6::Quick BobUI6::Sql BobUI6::Network BobUI6::WebSockets BobUI6::Concurrent BobUI6::Multimedia
>>>>>>> origin/dev
)

if(EMSCRIPTEN)
    target_link_options(HelloOmni PRIVATE -s USE_PTHREADS=0)
endif()
<<<<<<< HEAD
<<<<<<< HEAD
    Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Qml Qt6::Quick
=======
    BobUI6::Core BobUI6::Gui BobUI6::Widgets BobUI6::Qml BobUI6::Quick
>>>>>>> origin/dev
)
=======
>>>>>>> feature/omni-ui-framework-18001284211800334382
