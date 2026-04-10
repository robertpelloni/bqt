# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## QODBCDriverPlugin Plugin:
#####################################################################

bobui_find_package(ODBC)

bobui_internal_add_plugin(QODBCDriverPlugin
    OUTPUT_NAME qsqlodbc
    PLUGIN_TYPE sqldrivers
    SOURCES
        main.cpp
        qsql_odbc.cpp qsql_odbc_p.h
    DEFINES
        BOBUI_NO_CAST_FROM_ASCII
        BOBUI_NO_CAST_TO_ASCII
        BOBUI_NO_CONTEXTLESS_CONNECT
    LIBRARIES
        ODBC::ODBC
        BobUI::Core
        BobUI::CorePrivate
        BobUI::SqlPrivate
)

## Scopes:
#####################################################################

bobui_internal_extend_target(QODBCDriverPlugin CONDITION UNIX
    DEFINES
        UNICODE
)
