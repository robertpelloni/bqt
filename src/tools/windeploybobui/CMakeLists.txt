# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## windeploybobui Tool:
#####################################################################

bobui_get_tool_target_name(target_name windeploybobui)
bobui_internal_add_tool(${target_name}
    TOOLS_TARGET Core
    USER_FACING
    INSTALL_VERSIONED_LINK
    TARGET_DESCRIPTION "BobUI Windows Deployment Tool"
    SOURCES
        qmlutils.cpp qmlutils.h
        bobuimoduleinfo.cpp bobuimoduleinfo.h
        bobuiplugininfo.cpp bobuiplugininfo.h
        utils.cpp utils.h
        peheaderinfo.cpp peheaderinfo.h
        main.cpp
    DEFINES
        BOBUI_NO_CAST_FROM_ASCII
        BOBUI_NO_CAST_TO_ASCII
        BOBUI_NO_FOREACH
        BOBUI_NO_QPAIR
        BOBUI_NO_URL_CAST_FROM_STRING
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Network
)

bobui_internal_return_unless_building_tools()

bobui_internal_extend_target(${target_name} CONDITION WIN32
    PUBLIC_LIBRARIES
        shlwapi
)
