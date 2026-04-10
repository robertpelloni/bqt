# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(sliders LANGUAGES CXX)

find_package(BobUI6 REQUIRED COMPONENTS Core Gui Widgets)

bobui_standard_project_setup()

bobui_add_executable(sliders
    main.cpp
    slidersgroup.cpp slidersgroup.h
    window.cpp window.h
)

set_target_properties(sliders PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(sliders PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Widgets
)

install(TARGETS sliders
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET sliders
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
