# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(secureudpserver LANGUAGES CXX)

find_package(BobUI6 REQUIRED COMPONENTS Core Gui Network Widgets)

bobui_standard_project_setup()

bobui_add_executable(secureudpserver
    main.cpp
    mainwindow.cpp mainwindow.h mainwindow.ui
    nicselector.cpp nicselector.h nicselector.ui
    server.cpp server.h
)

set_target_properties(secureudpserver PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(secureudpserver PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Network
    BobUI6::Widgets
)

install(TARGETS secureudpserver
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET secureudpserver
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
