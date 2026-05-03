# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(blurpicker LANGUAGES CXX)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/widgets/effects/blurpicker")

find_package(BobUI6 REQUIRED COMPONENTS Core Gui Widgets)

bobui_standard_project_setup()

bobui_add_executable(blurpicker
    blureffect.cpp blureffect.h
    blurpicker.cpp blurpicker.h
    main.cpp
)

set_target_properties(blurpicker PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(blurpicker PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Widgets
)

# Resources:
set(blurpicker_resource_files
    "images/accessories-calculator.png"
    "images/accessories-text-editor.png"
    "images/background.jpg"
    "images/help-browser.png"
    "images/internet-group-chat.png"
    "images/internet-mail.png"
    "images/internet-web-browser.png"
    "images/office-calendar.png"
    "images/system-users.png"
)

bobui_add_resources(blurpicker "blurpicker"
    PREFIX
        "/"
    FILES
        ${blurpicker_resource_files}
)

install(TARGETS blurpicker
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
