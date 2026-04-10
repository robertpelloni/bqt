# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(chart LANGUAGES CXX)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/widgets/itemviews/chart")

find_package(BobUI6 REQUIRED COMPONENTS Core Gui Widgets)

bobui_standard_project_setup()

bobui_add_executable(chart
    main.cpp
    mainwindow.cpp mainwindow.h
    pieview.cpp pieview.h
)

set_target_properties(chart PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(chart PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Widgets
)

# Resources:
set(chart_resource_files
    "bobuidata.cht"
)

bobui_add_resources(chart "chart"
    PREFIX
        "/Charts"
    FILES
        ${chart_resource_files}
)

if(UNIX AND NOT APPLE AND NOT HAIKU AND NOT INTEGRITY AND NOT VXWORKS)
    target_link_libraries(chart PRIVATE
        m
    )
endif()

install(TARGETS chart
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
