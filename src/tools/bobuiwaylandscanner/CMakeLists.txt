# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Generated from bobuiwaylandscanner.pro.

#####################################################################
## bobuiwaylandscanner Tool:
#####################################################################

bobui_get_tool_target_name(target_name bobuiwaylandscanner)
bobui_internal_add_tool(${target_name}
    TOOLS_TARGET WaylandScanner # special case
    INSTALL_DIR "${INSTALL_LIBEXECDIR}"
    SOURCES
        bobuiwaylandscanner.cpp
    EXTRA_CMAKE_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/BobUI6WaylandClientMacros.cmake"
        "${CMAKE_CURRENT_SOURCE_DIR}/BobUI6WaylandCompositorMacros.cmake"
    EXTRA_CMAKE_INCLUDES
        "BobUI6WaylandClientMacros.cmake"
        "BobUI6WaylandCompositorMacros.cmake"
)
bobui_internal_return_unless_building_tools()

#### Keys ignored in scope 1:.:.:bobuiwaylandscanner.pro:<TRUE>:
# _OPTION = "host_build"

# special case begin
# Abuse the function to make sure the package providing bobuiwaylandscanner calls
# find_package(WaylandScanner), aka the non-bobui provided package.
bobui_record_extra_package_dependency(bobuiwaylandscanner WaylandScanner "")
# special case end
