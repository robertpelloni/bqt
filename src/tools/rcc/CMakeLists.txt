# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## rcc Tool:
#####################################################################

bobui_get_tool_target_name(target_name rcc)
bobui_internal_add_tool(${target_name}
    TRY_RUN
    TARGET_DESCRIPTION "BobUI Resource Compiler"
    INSTALL_DIR "${INSTALL_LIBEXECDIR}"
    TOOLS_TARGET Core
    SOURCES
        main.cpp
        rcc.cpp rcc.h
    DEFINES
        BOBUI_NO_CAST_FROM_ASCII
        BOBUI_NO_FOREACH
        BOBUI_RCC
        BOBUI_USE_NODISCARD_FILE_OPEN
        BOBUI_NO_URL_CAST_FROM_STRING
    INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}
)
bobui_internal_return_unless_building_tools()

## Scopes:
#####################################################################

bobui_internal_extend_target(${target_name} CONDITION BOBUI_FEATURE_zstd
    LIBRARIES
        WrapZSTD::WrapZSTD
)
