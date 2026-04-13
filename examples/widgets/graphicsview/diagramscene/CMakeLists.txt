# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(diagramscene LANGUAGES CXX)

find_package(BobUI6 REQUIRED COMPONENTS Core Gui Widgets)

bobui_standard_project_setup()

bobui_add_executable(diagramscene
    arrow.cpp arrow.h
    diagramitem.cpp diagramitem.h
    diagramscene.cpp diagramscene.h
    diagramtextitem.cpp diagramtextitem.h
    main.cpp
    mainwindow.cpp mainwindow.h
)

set_target_properties(diagramscene PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(diagramscene PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Widgets
)

# Resources:
set(diagramscene_resource_files
    "images/background1.png"
    "images/background2.png"
    "images/background3.png"
    "images/background4.png"
    "images/bold.png"
    "images/bringtofront.png"
    "images/delete.png"
    "images/floodfill.png"
    "images/italic.png"
    "images/linecolor.png"
    "images/linepointer.png"
    "images/pointer.png"
    "images/sendtoback.png"
    "images/textpointer.png"
    "images/underline.png"
)

bobui_add_resources(diagramscene "diagramscene"
    PREFIX
        "/"
    FILES
        ${diagramscene_resource_files}
)

install(TARGETS diagramscene
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET diagramscene
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
