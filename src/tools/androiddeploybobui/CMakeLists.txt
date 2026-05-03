# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## androiddeploybobui App:
#####################################################################

bobui_get_tool_target_name(target_name androiddeploybobui)
bobui_internal_add_tool(${target_name}
    TARGET_DESCRIPTION "BobUI Android Deployment Tool"
    TOOLS_TARGET Core
    USER_FACING
    INSTALL_VERSIONED_LINK
    SOURCES
        main.cpp
    DEFINES
        BOBUI_NO_CAST_FROM_ASCII
        BOBUI_NO_CAST_TO_ASCII
        BOBUI_NO_FOREACH
        BOBUI_NO_QPAIR
        BOBUI_NO_URL_CAST_FROM_STRING
    LIBRARIES
        BobUI::Core
    INCLUDE_DIRECTORIES
        ../shared
)
bobui_internal_return_unless_building_tools()
set_target_properties(${target_name} PROPERTIES
    WIN32_EXECUTABLE FALSE
)

## Scopes:
#####################################################################

bobui_internal_extend_target(${target_name} CONDITION windows
    COMPILE_OPTIONS
        -U__STRICT_ANSI__
)
