project(OmniMLTrainer)

find_package(BobUI6 COMPONENTS Core Gui Qml Quick REQUIRED)

bobui_add_executable(OmniMLTrainer
    src/main.cpp
    ../../core/src/OmniApp.cpp
    ../../core/src/ml/OmniNeuralNetwork.cpp
)

target_include_directories(OmniMLTrainer PRIVATE
    ../../core/include
    ../../deps/juce
)

target_link_libraries(OmniMLTrainer PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Qml
    BobUI6::Quick
)

bobui_add_qml_module(OmniMLTrainer
    URI Omni.ML
    VERSION 1.0
    QML_FILES assets/main.qml
)
