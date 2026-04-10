# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## qvkgen Tool:
#####################################################################

bobui_get_tool_target_name(target_name qvkgen)
bobui_internal_add_tool(${target_name}
    REQUIRED_FOR_DOCS
    TARGET_DESCRIPTION "BobUI Vulkan Header Generator"
    INSTALL_DIR "${INSTALL_LIBEXECDIR}"
    TOOLS_TARGET Gui
    SOURCES
        qvkgen.cpp
    LIBRARIES
        BobUI::Core
)
bobui_internal_return_unless_building_tools()
