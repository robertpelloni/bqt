# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tracegen Tool:
#####################################################################

bobui_get_tool_target_name(target_name tracegen)
bobui_internal_add_tool(${target_name}
    CORE_LIBRARY Bootstrap
    INSTALL_DIR "${INSTALL_LIBEXECDIR}"
    TOOLS_TARGET Core
    SOURCES
        etw.cpp etw.h
        helpers.cpp helpers.h
        ctf.cpp ctf.h
        lttng.cpp lttng.h
        panic.cpp panic.h
        provider.cpp provider.h
        bobuiheaders.cpp bobuiheaders.h
        tracegen.cpp
    NO_UNITY_BUILD
)
bobui_internal_return_unless_building_tools()
