# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(draggableicons LANGUAGES CXX)

find_package(BobUI6 REQUIRED COMPONENTS Core Gui Widgets)

bobui_standard_project_setup()

bobui_add_executable(draggableicons
    dragwidget.cpp dragwidget.h
    main.cpp
)

set_target_properties(draggableicons PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(draggableicons PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Widgets
)

if(EMSCRIPTEN)
    target_link_options(draggableicons PUBLIC -sASYNCIFY -Os)
endif()


# Resources:
set(draggableicons_resource_files
    "images/boat.png"
    "images/car.png"
    "images/house.png"
)

bobui_add_resources(draggableicons "draggableicons"
    PREFIX
        "/"
    FILES
        ${draggableicons_resource_files}
)

install(TARGETS draggableicons
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET draggableicons
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
