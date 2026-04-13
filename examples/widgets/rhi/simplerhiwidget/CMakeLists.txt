# Copyright (C) 2023 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(simplerhiwidget LANGUAGES CXX)

find_package(BobUI6 REQUIRED COMPONENTS Core GuiPrivate Widgets)

bobui_standard_project_setup()

bobui_add_executable(simplerhiwidget
    examplewidget.cpp examplewidget.h
    main.cpp
)

set_target_properties(simplerhiwidget PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

# needs GuiPrivate to be able to include <rhi/qrhi.h>
target_link_libraries(simplerhiwidget PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::GuiPrivate
    BobUI6::Widgets
)

bobui_add_resources(simplerhiwidget "simplerhiwidget"
    PREFIX
        "/"
    FILES
        "shader_assets/color.vert.qsb"
        "shader_assets/color.frag.qsb"
)

install(TARGETS simplerhiwidget
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET simplerhiwidget
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
