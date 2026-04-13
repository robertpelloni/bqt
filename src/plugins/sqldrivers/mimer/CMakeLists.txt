# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause
# Generated from mimer.pro.

#####################################################################
## MIMERSQLDriverPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QMimerSQLDriverPlugin
    OUTPUT_NAME qsqlmimer
    PLUGIN_TYPE sqldrivers
    SOURCES
        main.cpp
        qsql_mimer.cpp qsql_mimer.h
    DEFINES
        BOBUI_NO_CAST_FROM_ASCII
        BOBUI_NO_CAST_TO_ASCII
        BOBUI_NO_CONTEXTLESS_CONNECT
    LIBRARIES
        MimerSQL::MimerSQL
        BobUI::Core
        BobUI::SqlPrivate
)

#### Keys ignored in scope 1:.:.:mimer.pro:<TRUE>:
# OTHER_FILES = "mimer.json"
