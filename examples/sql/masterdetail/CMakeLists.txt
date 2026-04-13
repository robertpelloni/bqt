# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(masterdetail LANGUAGES CXX)

find_package(BobUI6 REQUIRED COMPONENTS Core Gui Sql Widgets Xml)

bobui_standard_project_setup()

bobui_add_executable(masterdetail
    database.h
    dialog.cpp dialog.h
    main.cpp
    mainwindow.cpp mainwindow.h
)

set_target_properties(masterdetail PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(masterdetail PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Sql
    BobUI6::Widgets
    BobUI6::Xml
)

# Resources:
set(masterdetail_resource_files
    "images/icon.png"
    "images/image.png"
    "albumdetails.xml"
)

bobui_add_resources(masterdetail "masterdetail"
    PREFIX
        "/"
    FILES
        ${masterdetail_resource_files}
)

install(TARGETS masterdetail
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET masterdetail
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
