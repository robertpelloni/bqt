# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(rasterwindow LANGUAGES CXX)

find_package(BobUI6 REQUIRED COMPONENTS Core Gui)

bobui_standard_project_setup()

bobui_add_executable(rasterwindow
    main.cpp
    rasterwindow.cpp rasterwindow.h
)

set_target_properties(rasterwindow PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(rasterwindow PRIVATE
    BobUI6::Core
    BobUI6::Gui
)

install(TARGETS rasterwindow
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET rasterwindow
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
