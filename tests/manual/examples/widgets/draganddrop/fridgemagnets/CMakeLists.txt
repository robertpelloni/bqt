# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(fridgemagnets LANGUAGES CXX)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/widgets/draganddrop/fridgemagnets")

find_package(BobUI6 REQUIRED COMPONENTS Core Gui Widgets)

bobui_standard_project_setup()

bobui_add_executable(fridgemagnets
    draglabel.cpp draglabel.h
    dragwidget.cpp dragwidget.h
    main.cpp
)

set_target_properties(fridgemagnets PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(fridgemagnets PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Widgets
)

# Resources:
set(fridgemagnets_resource_files
    "words.txt"
)

bobui_add_resources(fridgemagnets "fridgemagnets"
    PREFIX
        "/dictionary"
    FILES
        ${fridgemagnets_resource_files}
)

install(TARGETS fridgemagnets
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
