# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause


cmake_minimum_required(VERSION 3.16)

project(no_link_gui)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/FindPackageHints.cmake")
    include("${CMAKE_CURRENT_LIST_DIR}/FindPackageHints.cmake")
endif()

find_package(BobUI6Gui REQUIRED HINTS ${BobUI6Tests_PREFIX_PATH})
find_package(BobUI6Test REQUIRED HINTS ${BobUI6Tests_PREFIX_PATH})

include_directories(
    ${BobUI6Gui_INCLUDE_DIRS}
    ${BobUI6Test_INCLUDE_DIRS}
)

add_compile_definitions(
    ${BobUI6Gui_DEFINITIONS}
    ${BobUI6Test_DEFINITIONS}
)

set(main_file "main.cpp")
set(moc_file "${CMAKE_CURRENT_BINARY_DIR}/main.moc")

bobui_generate_moc("${main_file}" "${moc_file}")

# The core_test is expected to fail to build because
# BOBUI_GUI_LIB is defined, which affects the contents of
# BobUI Test and the definition of BOBUIEST_MAIN.
# If running this test manually (ctest -V -R no_link_gui from
# the tests/auto/cmake/build directory), the core_test is
# expected to fail to link because of missing symbols from BobUI GUI.
# The gui_test is expected to build successfully  (though it may
# be necessary to comment out the core_test and re-run cmake)
add_executable(core_test "${main_file}" "${moc_file}")
target_link_libraries(core_test
    ${BobUI6Core_LIBRARIES}
    ${BobUI6Test_LIBRARIES}
)

add_executable(gui_test "${main_file}" "${moc_file}")
target_link_libraries(gui_test
    ${BobUI6Gui_LIBRARIES}
    ${BobUI6Test_LIBRARIES}
)
