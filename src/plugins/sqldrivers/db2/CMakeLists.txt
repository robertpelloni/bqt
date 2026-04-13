# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## QDB2DriverPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QDB2DriverPlugin
    OUTPUT_NAME qsqldb2
    PLUGIN_TYPE sqldrivers
    SOURCES
        main.cpp
        qsql_db2.cpp qsql_db2_p.h
    DEFINES
        BOBUI_NO_CAST_FROM_ASCII
        BOBUI_NO_CAST_TO_ASCII
    LIBRARIES
        DB2::DB2
        BobUI::Core
        BobUI::CorePrivate
        BobUI::SqlPrivate
)

## Scopes:
#####################################################################

bobui_internal_extend_target(QDB2DriverPlugin CONDITION (TEST_architecture_arch STREQUAL "x86_64")
    DEFINES
        ODBC64
)

bobui_internal_force_macos_intel_arch(QDB2DriverPlugin)
