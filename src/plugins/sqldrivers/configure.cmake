# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause



#### Inputs

# input sqlite
set(INPUT_sqlite "undefined" CACHE STRING "")
set_property(CACHE INPUT_sqlite PROPERTY STRINGS undefined bobui system)



#### Libraries

bobui_feature_vcpkg_scope(sql)
bobui_find_package(DB2 MODULE PROVIDED_TARGETS DB2::DB2 MODULE_NAME sqldrivers QMAKE_LIB db2)
bobui_find_package(MySQL MODULE PROVIDED_TARGETS MySQL::MySQL MODULE_NAME sqldrivers QMAKE_LIB mysql
    VCPKG_PORT libmysql
    VCPKG_ADD_TO_FEATURE sql-mysql
)
bobui_find_package(PostgreSQL MODULE PROVIDED_TARGETS PostgreSQL::PostgreSQL MODULE_NAME sqldrivers QMAKE_LIB psql
    VCPKG_PORT libpq
    VCPKG_ADD_TO_FEATURE sql-psql
)
bobui_find_package(Oracle MODULE PROVIDED_TARGETS Oracle::OCI MODULE_NAME sqldrivers QMAKE_LIB oci)
bobui_find_package(ODBC PROVIDED_TARGETS ODBC::ODBC MODULE_NAME sqldrivers QMAKE_LIB odbc)
bobui_find_package(SQLite3 PROVIDED_TARGETS SQLite::SQLite3 MODULE_NAME sqldrivers QMAKE_LIB sqlite3
    VCPKG_PORT sqlite3
    VCPKG_ADD_TO_FEATURE sql-sqlite
)
bobui_find_package(Interbase MODULE
    PROVIDED_TARGETS Interbase::Interbase MODULE_NAME sqldrivers QMAKE_LIB ibase) # special case
bobui_find_package(Mimer MODULE PROVIDED_TARGETS MimerSQL::MimerSQL MODULE_NAME sqldrivers QMAKE_LIB mimer)
if(NOT WIN32 AND BOBUI_FEATURE_system_zlib)
    bobui_add_qmake_lib_dependency(sqlite3 zlib)
endif()


#### Tests



#### Features

bobui_feature("sql-db2" PRIVATE
    LABEL "DB2 (IBM)"
    CONDITION DB2_FOUND
)
bobui_feature("sql-ibase" PRIVATE
    LABEL "InterBase"
    CONDITION Interbase_FOUND # special case
)
bobui_feature("sql-mysql" PRIVATE
    LABEL "MySql"
    CONDITION MySQL_FOUND
)
bobui_feature("sql-oci" PRIVATE
    LABEL "OCI (Oracle)"
    CONDITION Oracle_FOUND
)
bobui_feature("sql-odbc" PRIVATE
    LABEL "ODBC"
    CONDITION BOBUI_FEATURE_datestring AND ODBC_FOUND
)
bobui_feature("sql-psql" PRIVATE
    LABEL "PostgreSQL"
    CONDITION PostgreSQL_FOUND
)
bobui_feature("sql-sqlite" PRIVATE
    LABEL "SQLite"
    CONDITION BOBUI_FEATURE_datestring
    VCPKG_DEFAULT
)
bobui_feature("system-sqlite" PRIVATE SYSTEM_LIBRARY
    LABEL "  Using system provided SQLite"
    AUTODETECT OFF
    CONDITION BOBUI_FEATURE_sql_sqlite AND SQLite3_FOUND
)
bobui_feature("sql-mimer" PRIVATE
    LABEL "Mimer"
    CONDITION Mimer_FOUND
)

bobui_configure_add_summary_section(NAME "BobUI Sql Drivers")
bobui_configure_add_summary_entry(ARGS "sql-db2")
bobui_configure_add_summary_entry(ARGS "sql-ibase")
bobui_configure_add_summary_entry(ARGS "sql-mysql")
bobui_configure_add_summary_entry(ARGS "sql-oci")
bobui_configure_add_summary_entry(ARGS "sql-odbc")
bobui_configure_add_summary_entry(ARGS "sql-psql")
bobui_configure_add_summary_entry(ARGS "sql-sqlite")
bobui_configure_add_summary_entry(ARGS "system-sqlite")
bobui_configure_add_summary_entry(ARGS "sql-mimer")
bobui_configure_end_summary_section() # end of "BobUI Sql Drivers" section
bobui_configure_add_report_entry(
    TYPE WARNING
    MESSAGE "BobUI does not support compiling the Oracle database driver with MinGW, due to lack of such support from Oracle. Consider disabling the Oracle driver, as the current build will most likely fail."
    CONDITION WIN32 AND NOT MSVC AND BOBUI_FEATURE_sql_oci
)
