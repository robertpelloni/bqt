# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tracepointgen Tool:
#####################################################################

bobui_get_tool_target_name(target_name tracepointgen)
bobui_internal_add_tool(${target_name}
    CORE_LIBRARY Bootstrap
    INSTALL_DIR "${INSTALL_LIBEXECDIR}"
    TOOLS_TARGET Core
    SOURCES
        tracepointgen.cpp tracepointgen.h
        parser.cpp parser.h
)
bobui_internal_return_unless_building_tools()
