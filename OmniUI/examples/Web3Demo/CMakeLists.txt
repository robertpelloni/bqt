project(Web3Demo)

find_package(BobUI6 COMPONENTS Core Gui Qml Quick REQUIRED)

bobui_add_executable(Web3Demo
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
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Qml
    BobUI6::Quick
)

bobui_add_qml_module(Web3Demo
    URI Omni.Web3
    VERSION 1.0
    QML_FILES assets/main.qml
)
