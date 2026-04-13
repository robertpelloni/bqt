# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
cmake_minimum_required(VERSION 3.16)
project(shortcuteditor LANGUAGES CXX)

find_package(BobUI6 REQUIRED COMPONENTS Core Gui Widgets)

bobui_standard_project_setup()

bobui_add_executable(shortcuteditor
    actionmanager.cpp actionmanager.h
    application.cpp application.h
    main.cpp
    mainwindow.cpp mainwindow.h
    shortcuteditordelegate.cpp shortcuteditordelegate.h
    shortcuteditormodel.cpp shortcuteditormodel.h
    shortcuteditorwidget.cpp shortcuteditorwidget.h
)

set_target_properties(shortcuteditor PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(shortcuteditor PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Widgets
)

install(TARGETS shortcuteditor
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET shortcuteditor
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
