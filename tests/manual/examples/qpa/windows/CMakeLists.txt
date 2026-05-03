# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(windows LANGUAGES CXX)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/qpa/windows")

find_package(BobUI6 REQUIRED COMPONENTS Core Gui)

bobui_standard_project_setup()

bobui_add_executable(windows
    main.cpp
    window.cpp window.h
)

set_target_properties(windows PROPERTIES
    WIN32_EXECUTABLE FALSE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(windows PRIVATE
    BobUI6::Core
    BobUI6::CorePrivate
    BobUI6::Gui
    BobUI6::GuiPrivate
)

install(TARGETS windows
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
