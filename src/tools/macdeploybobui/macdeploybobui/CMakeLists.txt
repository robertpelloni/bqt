# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## macdeploybobui Tool:
#####################################################################

bobui_get_tool_target_name(target_name macdeploybobui)
bobui_internal_add_tool(${target_name}
    TOOLS_TARGET Core
    USER_FACING
    INSTALL_VERSIONED_LINK
    TARGET_DESCRIPTION "BobUI macOS Deployment Tool"
    SOURCES
        ../shared/shared.cpp
        main.cpp
    DEFINES
        BOBUI_NO_FOREACH
        BOBUI_USE_NODISCARD_FILE_OPEN
    LIBRARIES
        ${FWCoreFoundation}
)
bobui_internal_return_unless_building_tools()
