# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## bobuipaths App:
#####################################################################

bobui_get_tool_target_name(target_name bobuipaths)
bobui_internal_add_tool(${target_name}
    TRY_RUN
    TARGET_DESCRIPTION "BobUI tool that provides the standard paths of the BobUI framework"
    TOOLS_TARGET Core
    INSTALL_VERSIONED_LINK
    SOURCES
        bobuipaths.cpp
    DEFINES
        BOBUIPATHS_VERSION_STR="2.0"
)
bobui_internal_return_unless_building_tools()

## Scopes:
#####################################################################

bobui_internal_extend_target(${target_name} CONDITION BOBUI_FEATURE_settings
    LIBRARIES
        BobUILibraryInfo
)

if(WIN32 AND TARGET ${target_name})
    set_target_properties(${target_name} PROPERTIES
        WIN32_EXECUTABLE FALSE
    )
endif()
