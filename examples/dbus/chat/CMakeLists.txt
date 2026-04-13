# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(chat LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(BobUI6 REQUIRED COMPONENTS Core DBus Gui Widgets)

bobui_standard_project_setup()

set(chat_SRCS)
bobui_add_dbus_interface(chat_SRCS
    org.example.chat.xml
    chat_interface
)

bobui_add_dbus_adaptor(chat_SRCS
    org.example.chat.xml
    qobject.h
    QObject
    chat_adaptor
)

bobui_add_executable(chat
    chat.cpp chat.h
    chatmainwindow.ui
    ${chat_SRCS}
)

set_target_properties(chat PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(chat PRIVATE
    BobUI6::Core
    BobUI6::DBus
    BobUI6::Gui
    BobUI6::Widgets
)

install(TARGETS chat
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET chat
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
