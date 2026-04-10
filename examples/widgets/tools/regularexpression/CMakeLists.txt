# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(regularexpression LANGUAGES CXX)

find_package(BobUI6 REQUIRED COMPONENTS Core Gui Widgets)

bobui_standard_project_setup()

bobui_add_executable(regularexpression
    main.cpp
    regularexpressiondialog.cpp regularexpressiondialog.h
)

set_target_properties(regularexpression PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(regularexpression PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Widgets
)

# Resources:
set(regularexpression_resource_files
    "images/copy.png"
)

bobui_add_resources(regularexpression "regularexpression"
    PREFIX
        "/"
    FILES
        ${regularexpression_resource_files}
)

install(TARGETS regularexpression
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET regularexpression
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
