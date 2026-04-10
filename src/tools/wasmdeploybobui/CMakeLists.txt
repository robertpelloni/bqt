# Copyright (C) 2025 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## wasmdeploybobui Tool:
#####################################################################

bobui_get_tool_target_name(target_name wasmdeploybobui)
bobui_internal_add_tool(${target_name}
    TOOLS_TARGET Core
    USER_FACING
    INSTALL_VERSIONED_LINK
    TARGET_DESCRIPTION "BobUI WebAssembly Deployment Tool"
    SOURCES
        main.cpp wasmbinary.cpp jsontools.cpp
    LIBRARIES
        BobUI::CorePrivate
)
bobui_internal_return_unless_building_tools()
