# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(sqlbrowser LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(BobUI6 REQUIRED COMPONENTS Core Gui Sql Widgets)

bobui_standard_project_setup()

bobui_add_executable(sqlbrowser
    browser.cpp browser.h
    browserwidget.ui
    connectionwidget.cpp connectionwidget.h
    main.cpp
    qsqlconnectiondialog.cpp qsqlconnectiondialog.h qsqlconnectiondialog.ui
)

set_target_properties(sqlbrowser PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(sqlbrowser PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Sql
    BobUI6::Widgets
)

install(TARGETS sqlbrowser
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET sqlbrowser
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
