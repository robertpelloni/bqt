# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(classwizard LANGUAGES CXX)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/widgets/dialogs/classwizard")

find_package(BobUI6 REQUIRED COMPONENTS Core Gui Widgets)

bobui_standard_project_setup()

bobui_add_executable(classwizard
    classwizard.cpp classwizard.h
    main.cpp
)

set_target_properties(classwizard PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(classwizard PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Widgets
)

# Resources:
set(classwizard_resource_files
    "images/background.png"
    "images/banner.png"
    "images/logo1.png"
    "images/logo2.png"
    "images/logo3.png"
    "images/watermark1.png"
    "images/watermark2.png"
)

bobui_add_resources(classwizard "classwizard"
    PREFIX
        "/"
    FILES
        ${classwizard_resource_files}
)

install(TARGETS classwizard
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
