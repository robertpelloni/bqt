# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(hellovulkancubes LANGUAGES CXX)

find_package(BobUI6 REQUIRED COMPONENTS Concurrent Core Gui Widgets)

bobui_standard_project_setup()

bobui_add_executable(hellovulkancubes
    camera.cpp camera.h
    main.cpp
    mainwindow.cpp mainwindow.h
    mesh.cpp mesh.h
    renderer.cpp renderer.h
    shader.cpp shader.h
    vulkanwindow.cpp vulkanwindow.h
)

set_target_properties(hellovulkancubes PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(hellovulkancubes PRIVATE
    BobUI6::Concurrent
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Widgets
)

# Resources:
set_source_files_properties("../shared/block.buf"
    PROPERTIES BOBUI_RESOURCE_ALIAS "block.buf"
)

set_source_files_properties("../shared/bobui_logo.buf"
    PROPERTIES BOBUI_RESOURCE_ALIAS "bobui_logo.buf"
)

set(hellovulkancubes_resource_files
    "../shared/block.buf"
    "../shared/bobui_logo.buf"
    "color_frag.spv"
    "color_phong_frag.spv"
    "color_phong_vert.spv"
    "color_vert.spv"
)

bobui6_add_resources(hellovulkancubes "hellovulkancubes"
    PREFIX
        "/"
    FILES
        ${hellovulkancubes_resource_files}
)

install(TARGETS hellovulkancubes
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET hellovulkancubes
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
