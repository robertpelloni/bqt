# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(tutorial5 LANGUAGES CXX)

find_package(BobUI6 REQUIRED COMPONENTS Core Gui Test Widgets)

bobui_standard_project_setup()

bobui_add_executable(tutorial5
    benchmarking.cpp
)

set_target_properties(tutorial5 PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(tutorial5 PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Test
    BobUI6::Widgets
)

install(TARGETS tutorial5
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET tutorial5
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
