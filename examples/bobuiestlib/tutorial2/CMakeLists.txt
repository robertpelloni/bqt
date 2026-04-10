# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(tutorial2 LANGUAGES CXX)

find_package(BobUI6 REQUIRED COMPONENTS Core Gui Test Widgets)

bobui_standard_project_setup()

bobui_add_executable(tutorial2
    testqstring.cpp
)

set_target_properties(tutorial2 PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(tutorial2 PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Test
    BobUI6::Widgets
)

install(TARGETS tutorial2
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET tutorial2
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
