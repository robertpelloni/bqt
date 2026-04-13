# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(drilldown LANGUAGES CXX)

find_package(BobUI6 REQUIRED COMPONENTS Core Gui Sql Widgets)

bobui_standard_project_setup()

bobui_add_executable(drilldown
    ../connection.h
    imageitem.cpp imageitem.h
    informationwindow.cpp informationwindow.h
    main.cpp
    view.cpp view.h
)

set_target_properties(drilldown PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(drilldown PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Sql
    BobUI6::Widgets
)

# Resources:
set(drilldown_resource_files
    "images/bobui-creator.png"
    "images/bobui-logo.png"
    "images/bobui-project.png"
    "images/bobui-quick.png"
)

bobui_add_resources(drilldown "drilldown"
    PREFIX
        "/"
    FILES
        ${drilldown_resource_files}
)

install(TARGETS drilldown
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET drilldown
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
