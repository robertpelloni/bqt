# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_plugin(QIBaseDriverPlugin
    OUTPUT_NAME qsqlibase
    PLUGIN_TYPE sqldrivers
    SOURCES
        main.cpp
        qsql_ibase.cpp qsql_ibase_p.h
    DEFINES
        BOBUI_NO_CAST_FROM_ASCII
        BOBUI_NO_CAST_TO_ASCII
        BOBUI_NO_CONTEXTLESS_CONNECT
    LIBRARIES
        Interbase::Interbase
        BobUI::Core
        BobUI::CorePrivate
        BobUI::SqlPrivate)

bobui_internal_force_macos_intel_arch(QIBaseDriverPlugin)
