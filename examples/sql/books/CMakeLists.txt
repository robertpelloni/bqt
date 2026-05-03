# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(books LANGUAGES CXX)

find_package(BobUI6 REQUIRED COMPONENTS Core Gui Sql Widgets)

bobui_standard_project_setup()

bobui_add_executable(books
    bookdelegate.cpp bookdelegate.h
    bookwindow.cpp bookwindow.h
    initdb.h
    main.cpp
)

set_target_properties(books PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(books PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Sql
    BobUI6::Widgets
)

# Resources:
set(books_resource_files
    "images/star.svg"
    "images/star-filled.svg"
)

bobui_add_resources(books "books"
    PREFIX
        "/"
    FILES
        ${books_resource_files}
)

install(TARGETS books
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET books
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
