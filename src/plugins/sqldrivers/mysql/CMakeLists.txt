# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## QMYSQLDriverPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QMYSQLDriverPlugin
    OUTPUT_NAME qsqlmysql
    PLUGIN_TYPE sqldrivers
    SOURCES
        main.cpp
        qsql_mysql.cpp qsql_mysql_p.h
    DEFINES
        BOBUI_NO_CAST_FROM_ASCII
        BOBUI_NO_CAST_TO_ASCII
        BOBUI_NO_CONTEXTLESS_CONNECT
    LIBRARIES
        MySQL::MySQL
        BobUI::Core
        BobUI::CorePrivate
        BobUI::SqlPrivate
)

bobui_internal_force_macos_intel_arch(QMYSQLDriverPlugin)
