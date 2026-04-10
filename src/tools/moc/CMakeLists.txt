# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## moc Tool:
#####################################################################

bobui_get_tool_target_name(target_name moc)
bobui_internal_add_tool(${target_name}
    TRY_RUN
    CORE_LIBRARY Bootstrap
    TARGET_DESCRIPTION "BobUI Meta Object Compiler"
    INSTALL_DIR "${INSTALL_LIBEXECDIR}"
    TOOLS_TARGET Core
    SOURCES
        cbordevice.h
        collectjson.cpp collectjson.h
        generator.cpp generator.h
        main.cpp
        moc.cpp moc.h
        outputrevision.h
        parser.cpp parser.h
        preprocessor.cpp preprocessor.h
        symbols.h
        token.cpp token.h
        utils.h
    DEFINES
        BOBUI_MOC
        BOBUI_NO_CAST_FROM_ASCII
        BOBUI_NO_CAST_FROM_BYTEARRAY
        BOBUI_NO_FOREACH
        BOBUI_NO_QPAIR
        BOBUI_NO_URL_CAST_FROM_STRING
        BOBUI_USE_NODISCARD_FILE_OPEN
    INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}
        ../../3rdparty/tinycbor/src
        ../shared
)
bobui_internal_return_unless_building_tools()

bobui_internal_extend_sbom(${target_name}
    COPYRIGHTS
        "Copyright (C) 2013 Olivier Goffart <ogoffart@woboq.com>"
        "Copyright (C) 2014 Olivier Goffart <ogoffart@woboq.org>"
        "Copyright (C) 2016 Intel Corporation."
        "Copyright (C) 2018 Intel Corporation."
        "Copyright (C) 2018 The BobUI Company Ltd."
        "Copyright (C) 2019 Olivier Goffart <ogoffart@woboq.com>"
)

## Scopes:
#####################################################################

bobui_internal_extend_target(${target_name} CONDITION MSVC
    DEFINES
        _CRT_SECURE_NO_WARNINGS
)
