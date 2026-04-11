project(OmniMLTrainer)

find_package(Qt6 COMPONENTS Core Gui Qml Quick REQUIRED)

qt_add_executable(OmniMLTrainer
    src/main.cpp
    ../../core/src/OmniApp.cpp
    ../../core/src/ml/OmniNeuralNetwork.cpp
)

target_include_directories(OmniMLTrainer PRIVATE
    ../../core/include
    ../../deps/juce
)

target_link_libraries(OmniMLTrainer PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
)

qt_add_qml_module(OmniMLTrainer
    URI Omni.ML
    VERSION 1.0
    QML_FILES assets/main.qml
)
