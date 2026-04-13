# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(hellogles3 LANGUAGES CXX)

find_package(BobUI6 REQUIRED COMPONENTS Core Gui OpenGL)

bobui_standard_project_setup()

bobui_add_executable(hellogles3
    ../hellogl2/logo.cpp ../hellogl2/logo.h
    glwindow.cpp glwindow.h
    main.cpp
)

set_target_properties(hellogles3 PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(hellogles3 PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::OpenGL
)

# Resources:
set(hellogles3_resource_files
    "bobuilogo.png"
)

bobui_add_resources(hellogles3 "hellogles3"
    PREFIX
        "/"
    FILES
        ${hellogles3_resource_files}
)

install(TARGETS hellogles3
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET hellogles3
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
