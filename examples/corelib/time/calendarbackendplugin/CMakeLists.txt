# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(JulianGregorianCalendar VERSION 0.1 LANGUAGES CXX)

find_package(BobUI6 REQUIRED COMPONENTS CorePrivate Widgets)

bobui_standard_project_setup()

add_subdirectory(plugin)
add_subdirectory(application)

install(TARGETS calendarPlugin JulianGregorianCalendar
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET calendarPlugin
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

bobui_generate_deploy_app_script(
    TARGET JulianGregorianCalendar
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
