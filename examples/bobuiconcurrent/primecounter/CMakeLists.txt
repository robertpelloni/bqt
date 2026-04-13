# Copyright (C) 2023 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(primecounter LANGUAGES CXX)

find_package(BobUI6 REQUIRED COMPONENTS Concurrent Core Gui Widgets)

bobui_standard_project_setup()

bobui_add_executable(primecounter
    main.cpp
    primecounter.ui
    primecounter.cpp
    primecounter.h
)

set_target_properties(primecounter PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(primecounter PRIVATE
    BobUI6::Concurrent
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Widgets
)

install(TARGETS primecounter
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET primecounter
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
