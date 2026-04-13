# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## qdbuscpp2xml Tool:
#####################################################################

bobui_get_tool_target_name(target_name qdbuscpp2xml)
bobui_internal_add_tool(${target_name}
    TRY_RUN
    TRY_RUN_FLAGS "-V"
    TARGET_DESCRIPTION "BobUI D-Bus C++ to XML Compiler"
    TOOLS_TARGET DBus
    SOURCES
        ../moc/cbordevice.h
        ../moc/collectjson.cpp ../moc/collectjson.h
        ../moc/generator.cpp ../moc/generator.h
        ../moc/moc.cpp ../moc/moc.h
        ../moc/outputrevision.h
        ../moc/parser.cpp ../moc/parser.h
        ../moc/preprocessor.cpp ../moc/preprocessor.h
        ../moc/symbols.h
        ../moc/token.cpp ../moc/token.h
        ../moc/utils.h
        qdbuscpp2xml.cpp
    DEFINES
        BOBUI_NO_CAST_FROM_ASCII
        BOBUI_NO_FOREACH
        BOBUI_NO_URL_CAST_FROM_STRING
    INCLUDE_DIRECTORIES
        ../moc
        ../moc/../../3rdparty/tinycbor/src
    LIBRARIES
        BobUI::CorePrivate
        BobUI::DBusPrivate
)
bobui_internal_return_unless_building_tools()
