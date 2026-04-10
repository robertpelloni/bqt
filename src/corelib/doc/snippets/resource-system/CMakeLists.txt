# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

project(my_app)
cmake_minimum_required(VERSION 3.16)
find_package(BobUI6 REQUIRED COMPONENTS Widgets Qml)

#! [AUTORCC]
set(CMAKE_AUTORCC ON)

bobui_add_executable(my_app
    application.qrc
    main.cpp
)
#! [AUTORCC]

#! [bobui_add_resources]
bobui_add_resources(my_app "app_images"
    PREFIX "/"
    FILES
        images/copy.png
        images/cut.png
        images/new.png
        images/open.png
        images/paste.png
        images/save.png
)
#! [bobui_add_resources]

#! [bobui_add_big_resources]
bobui_add_big_resources(SOURCES application.qrc)
target_sources(my_app PRIVATE ${SOURCES})
#! [bobui_add_big_resources]

#! [bobui_add_binary_resources]
bobui_add_binary_resources(resources application.qrc DESTINATION application.rcc)
add_dependencies(my_app resources)
#! [bobui_add_binary_resources]

target_link_libraries(my_app
    PRIVATE BobUI6::Qml BobUI6::Widgets)
