# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(pixelator LANGUAGES CXX)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/widgets/itemviews/pixelator")

find_package(BobUI6
    REQUIRED COMPONENTS Core Gui Widgets
    OPTIONAL_COMPONENTS PrintSupport
)

bobui_standard_project_setup()

bobui_add_executable(pixelator
    imagemodel.cpp imagemodel.h
    main.cpp
    mainwindow.cpp mainwindow.h
    pixeldelegate.cpp pixeldelegate.h
)

set_target_properties(pixelator PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(pixelator PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Widgets
)

if(TARGET BobUI6::PrintSupport)
    target_link_libraries(pixelator PRIVATE BobUI6::PrintSupport)
endif()

# Resources:
set(images_resource_files
    "images/bobui.png"
)

bobui_add_resources(pixelator "images"
    PREFIX
        "/"
    FILES
        ${images_resource_files}
)

install(TARGETS pixelator
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
