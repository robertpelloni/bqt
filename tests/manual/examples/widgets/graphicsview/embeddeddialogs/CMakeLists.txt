# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(embeddeddialogs LANGUAGES CXX)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/widgets/graphicsview/embeddeddialogs")

find_package(BobUI6 REQUIRED COMPONENTS Core Gui Widgets)

bobui_standard_project_setup()

bobui_add_executable(embeddeddialogs
    customproxy.cpp customproxy.h
    embeddeddialog.cpp embeddeddialog.h embeddeddialog.ui
    main.cpp
)

set_target_properties(embeddeddialogs PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(embeddeddialogs PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Widgets
)

# Resources:
set(embeddeddialogs_resource_files
    "No-Ones-Laughing-3.jpg"
)

bobui_add_resources(embeddeddialogs "embeddeddialogs"
    PREFIX
        "/"
    FILES
        ${embeddeddialogs_resource_files}
)

install(TARGETS embeddeddialogs
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
