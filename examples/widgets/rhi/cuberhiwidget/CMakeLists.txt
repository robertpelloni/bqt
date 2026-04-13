# Copyright (C) 2023 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(cuberhiwidget LANGUAGES CXX)

find_package(BobUI6 REQUIRED COMPONENTS Core GuiPrivate Widgets)

bobui_standard_project_setup()

bobui_add_executable(cuberhiwidget
    examplewidget.cpp examplewidget.h cube.h
    main.cpp
)

set_target_properties(cuberhiwidget PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

# needs GuiPrivate to be able to include <rhi/qrhi.h>
target_link_libraries(cuberhiwidget PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::GuiPrivate
    BobUI6::Widgets
)

bobui_add_resources(cuberhiwidget "cuberhiwidget"
    PREFIX
        "/"
    FILES
        "shader_assets/texture.vert.qsb"
        "shader_assets/texture.frag.qsb"
)

install(TARGETS cuberhiwidget
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET cuberhiwidget
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
