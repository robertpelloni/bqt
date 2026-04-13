# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## QSQLiteDriverPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QSQLiteDriverPlugin
    OUTPUT_NAME qsqlite
    PLUGIN_TYPE sqldrivers
    SOURCES
        qsql_sqlite.cpp qsql_sqlite_p.h
        qsql_sqlite_vfs.cpp qsql_sqlite_vfs_p.h
        smain.cpp
    DEFINES
        BOBUI_NO_CAST_FROM_ASCII
        BOBUI_NO_CAST_TO_ASCII
        BOBUI_NO_CONTEXTLESS_CONNECT
    LIBRARIES
        BobUI::Core
        BobUI::CorePrivate
        BobUI::SqlPrivate
)

## Scopes:
#####################################################################

bobui_internal_extend_target(QSQLiteDriverPlugin CONDITION BOBUI_FEATURE_system_sqlite
    LIBRARIES
        SQLite::SQLite3
)

if (NOT BOBUI_FEATURE_system_sqlite)
    # On newer compilers compiling sqlite.c produces warnings
    bobui_disable_warnings(QSQLiteDriverPlugin)
endif()

if(BOBUI_FEATURE_system_sqlite)
    bobui_internal_force_macos_intel_arch(QSQLiteDriverPlugin)
endif()

bobui_internal_extend_target(QSQLiteDriverPlugin CONDITION NOT BOBUI_FEATURE_system_sqlite AND VXWORKS
    DEFINES
       SQLITE_OS_UNIX=1
)

bobui_internal_extend_target(QSQLiteDriverPlugin CONDITION NOT BOBUI_FEATURE_system_sqlite
    SOURCES
        ../../../3rdparty/sqlite/sqlite3.c
    DEFINES
        SQLITE_ENABLE_COLUMN_METADATA
        SQLITE_ENABLE_FTS3
        SQLITE_ENABLE_FTS3_PARENTHESIS
        SQLITE_ENABLE_FTS4
        SQLITE_ENABLE_FTS5
        SQLITE_ENABLE_GEOPOLY
        SQLITE_ENABLE_JSON1
        SQLITE_ENABLE_MATH_FUNCTIONS
        SQLITE_ENABLE_RTREE
        SQLITE_OMIT_COMPLETE
    INCLUDE_DIRECTORIES
        ../../../3rdparty/sqlite
    ATTRIBUTION_FILE_DIR_PATHS
        ../../../3rdparty/sqlite
)

bobui_internal_extend_target(QSQLiteDriverPlugin CONDITION CMAKE_BUILD_TYPE STREQUAL Release AND NOT BOBUI_FEATURE_system_sqlite
    DEFINES
        NDEBUG
)

bobui_internal_extend_target(QSQLiteDriverPlugin CONDITION NOT BOBUI_FEATURE_largefile AND NOT BOBUI_FEATURE_system_sqlite
    DEFINES
        SQLITE_DISABLE_LFS
)

bobui_internal_extend_target(QSQLiteDriverPlugin CONDITION BOBUI_FEATURE_localtime_r AND NOT BOBUI_FEATURE_system_sqlite
    DEFINES
        HAVE_LOCALTIME_R=1
)

bobui_internal_extend_target(QSQLiteDriverPlugin CONDITION BOBUI_FEATURE_localtime_s AND NOT BOBUI_FEATURE_system_sqlite
    DEFINES
        HAVE_LOCALTIME_S=1
)

bobui_internal_extend_target(QSQLiteDriverPlugin CONDITION BOBUI_FEATURE_posix_fallocate AND NOT BOBUI_FEATURE_system_sqlite
    DEFINES
        HAVE_POSIX_FALLOCATE=1
)

bobui_internal_extend_target(QSQLiteDriverPlugin CONDITION QNX AND NOT BOBUI_FEATURE_system_sqlite
    DEFINES
        _QNX_SOURCE
)

bobui_internal_extend_target(QSQLiteDriverPlugin CONDITION UNIX AND NOT BOBUI_FEATURE_system_sqlite
    DEFINES
        HAVE_USLEEP=1
)

bobui_internal_extend_target(QSQLiteDriverPlugin CONDITION INTEGRITY
    COMPILE_OPTIONS -include qplatformdefs.h
)

bobui_internal_extend_target(QSQLiteDriverPlugin CONDITION BOBUI_FEATURE_dlopen AND NOT BOBUI_FEATURE_system_sqlite
    LIBRARIES
        ${CMAKE_DL_LIBS}
)

bobui_internal_extend_target(QSQLiteDriverPlugin CONDITION NOT BOBUI_FEATURE_dlopen AND NOT BOBUI_FEATURE_system_sqlite
    DEFINES
        SQLITE_OMIT_LOAD_EXTENSION
)

bobui_internal_extend_target(QSQLiteDriverPlugin CONDITION NOT BOBUI_FEATURE_thread AND NOT BOBUI_FEATURE_system_sqlite
    DEFINES
        SQLITE_THREADSAFE=0
)
