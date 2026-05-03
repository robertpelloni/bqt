cmake_minimum_required(VERSION 3.16)

project(OmniUITests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(BobUI6 REQUIRED COMPONENTS Core Gui Test)

include_directories(../omnicore/include)

# Compile the specific core files needed for testing without pulling in the entire QML/GUI dependency graph
add_executable(tst_OmniInputManager
    omnicore/tst_OmniInputManager.cpp
    ../omnicore/src/OmniInputManager.cpp
)

target_link_libraries(tst_OmniInputManager PRIVATE
    BobUI6::Core BobUI6::Gui BobUI6::Test
)

enable_testing()
add_test(NAME tst_OmniInputManager COMMAND tst_OmniInputManager)
