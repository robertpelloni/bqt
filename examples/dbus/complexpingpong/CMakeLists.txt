# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(complexpingpong LANGUAGES CXX)

if (ANDROID)
    message(FATAL_ERROR "This project cannot be built on Android.")
endif()

find_package(BobUI6 REQUIRED COMPONENTS Core DBus)

bobui_standard_project_setup()

bobui_add_executable(complexping
    complexping.cpp complexping.h
    ping-common.h
)

target_link_libraries(complexping PRIVATE
    BobUI6::Core
    BobUI6::DBus
)

bobui_add_executable(complexpong
    complexpong.cpp complexpong.h
)

target_link_libraries(complexpong PRIVATE
    BobUI6::Core
    BobUI6::DBus
)

install(TARGETS complexping complexpong
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET complexping
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

bobui_generate_deploy_app_script(
    TARGET complexpong
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
