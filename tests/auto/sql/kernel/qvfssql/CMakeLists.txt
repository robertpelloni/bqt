# Copyright (C) 2023 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qsqlfield Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qvfssql LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qvfssql
    SOURCES
        tst_qvfssql.cpp
    LIBRARIES
        BobUI::SqlPrivate
)

set(qvfssql_resource_files
    "sample.db"
)

bobui_internal_add_resource(tst_qvfssql "tst_qvfssql"
    PREFIX
        "/ro/"
    FILES
        ${qvfssql_resource_files}
)
