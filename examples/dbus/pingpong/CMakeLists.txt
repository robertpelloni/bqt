# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(pingpong LANGUAGES CXX)

if (ANDROID)
    message(FATAL_ERROR "This project cannot be built on Android.")
endif()

find_package(BobUI6 REQUIRED COMPONENTS Core DBus)

bobui_standard_project_setup()

bobui_add_executable(ping
    ping.cpp
    ping-common.h
)

target_link_libraries(ping PRIVATE
    BobUI6::Core
    BobUI6::DBus
)

bobui_add_executable(pong
    ping-common.h
    pong.cpp
)

target_link_libraries(pong PRIVATE
    BobUI6::Core
    BobUI6::DBus
)

install(TARGETS ping pong
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET ping
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

bobui_generate_deploy_app_script(
    TARGET pong
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
