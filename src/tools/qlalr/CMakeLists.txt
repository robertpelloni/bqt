# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## qlalr Tool:
#####################################################################

bobui_get_tool_target_name(target_name qlalr)
bobui_internal_add_tool(${target_name}
    TARGET_DESCRIPTION "BobUI Look Ahead LR Parser Generator"
    INSTALL_DIR "${INSTALL_LIBEXECDIR}"
    TOOLS_TARGET Core
    SOURCES
        compress.cpp compress.h
        cppgenerator.cpp cppgenerator.h
        dotgraph.cpp dotgraph.h
        grammar.cpp grammar_p.h
        lalr.cpp lalr.h
        main.cpp
        parsetable.cpp parsetable.h
        recognizer.cpp recognizer.h
    DEFINES
        BOBUI_NO_FOREACH
        BOBUI_NO_QPAIR
        BOBUI_USE_NODISCARD_FILE_OPEN
        BOBUI_NO_URL_CAST_FROM_STRING
    LIBRARIES
        BobUI::Core
        BobUI::CorePrivate
)
bobui_internal_return_unless_building_tools()
