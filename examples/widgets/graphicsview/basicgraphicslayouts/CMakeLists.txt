# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(basicgraphicslayouts LANGUAGES CXX)

find_package(BobUI6 REQUIRED COMPONENTS Core Gui Widgets)

bobui_standard_project_setup()

bobui_add_executable(basicgraphicslayouts
    layoutitem.cpp layoutitem.h
    main.cpp
    window.cpp window.h
)

set_target_properties(basicgraphicslayouts PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(basicgraphicslayouts PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Widgets
)

# Resources:
set(basicgraphicslayouts_resource_files
    "images/block.png"
)

bobui_add_resources(basicgraphicslayouts "basicgraphicslayouts"
    PREFIX
        "/"
    FILES
        ${basicgraphicslayouts_resource_files}
)

install(TARGETS basicgraphicslayouts
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET basicgraphicslayouts
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
