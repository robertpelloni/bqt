# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## qdbusxml2cpp Tool:
#####################################################################

bobui_get_tool_target_name(target_name qdbusxml2cpp)
bobui_internal_add_tool(${target_name}
    REQUIRED_FOR_DOCS
    TRY_RUN
    TARGET_DESCRIPTION "BobUI D-Bus XML to C++ Compiler"
    TOOLS_TARGET DBus
    SOURCES
        qdbusxml2cpp.cpp
    DEFINES
        BOBUI_NO_CAST_FROM_ASCII
        BOBUI_NO_FOREACH
        BOBUI_NO_URL_CAST_FROM_STRING
    LIBRARIES
        BobUI::CorePrivate
        BobUI::DBusPrivate
)
bobui_internal_return_unless_building_tools()
