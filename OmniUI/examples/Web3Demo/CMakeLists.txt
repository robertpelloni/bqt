project(Web3Demo)

find_package(Qt6 COMPONENTS Core Gui Qml Quick REQUIRED)

qt_add_executable(Web3Demo
    src/main.cpp
    ../../core/src/OmniApp.cpp
    ../../core/src/web3/OmniWallet.cpp
    ../../core/src/web3/OmniTransaction.cpp
)

target_include_directories(Web3Demo PRIVATE
    ../../core/include
    ../../deps/juce
)

target_link_libraries(Web3Demo PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
)

qt_add_qml_module(Web3Demo
    URI Omni.Web3
    VERSION 1.0
    QML_FILES assets/main.qml
)
