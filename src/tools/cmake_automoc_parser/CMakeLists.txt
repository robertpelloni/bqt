# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## moc Tool:
#####################################################################

bobui_get_tool_target_name(target_name cmake_automoc_parser)
bobui_internal_add_tool(${target_name}
    CORE_LIBRARY Bootstrap
    TARGET_DESCRIPTION "BobUI CMake AUTOMOC Parser"
    INSTALL_DIR "${INSTALL_LIBEXECDIR}"
    TOOLS_TARGET Core
    SOURCES
        main.cpp
    DEFINES
        BOBUI_MOC
        BOBUI_NO_CAST_FROM_ASCII
        BOBUI_NO_CAST_FROM_BYTEARRAY
        BOBUI_NO_FOREACH
        BOBUI_NO_URL_CAST_FROM_STRING
    INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}
)
bobui_internal_return_unless_building_tools()
