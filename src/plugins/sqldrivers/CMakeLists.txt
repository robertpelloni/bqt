# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
if(NOT PROJECT_NAME STREQUAL "BobUIBase" AND NOT PROJECT_NAME STREQUAL "BobUI")
    include(.cmake.conf)
    # Store initial build type (if any is specified) to be read by
    # bobui_internal_set_cmake_build_type().
    # See bobui_internal_set_cmake_build_type() for details.
    if(DEFINED CACHE{CMAKE_BUILD_TYPE})
        set(__bobui_internal_standalone_project_cmake_build_type_before_project_call
            "${CMAKE_BUILD_TYPE}")
    endif()

    project(QSQLiteDriverPlugins
        VERSION "${BOBUI_REPO_MODULE_VERSION}"
        DESCRIPTION "BobUI6 SQL driver plugins"
        HOMEPAGE_URL "https://bobui.io/"
        LANGUAGES CXX C
    )
    find_package(BobUI6 ${PROJECT_VERSION} CONFIG REQUIRED COMPONENTS
        BuildInternals
        Core
        Sql
    )
    # Use bobuibase license files for standalone builds.
    set(BOBUI_SBOM_LICENSE_DIRS "${PROJECT_SOURCE_DIR}/../../../LICENSES")
    bobui_build_repo_begin()
else()
    bobui_internal_upgrade_cmake_policies()
endif()

# Currently handled completely manually.

# TODO sqldrivers_standalone {
#     _QMAKE_CACHE_ = $$shadowed($$SQLDRV_SRC_TREE)/.qmake.conf
#     load(bobui_configure)
# }

bobui_feature_module_begin(
    NO_MODULE
    PUBLIC_FILE "bobuisqldrivers-config.h"
    PRIVATE_FILE "bobuisqldrivers-config_p.h"
)
include(configure.cmake)
bobui_feature_module_end(NO_MODULE)


if(BOBUI_FEATURE_sql_psql AND BOBUI_FEATURE_regularexpression)
    add_subdirectory(psql)
endif()

if(BOBUI_FEATURE_sql_mysql)
     add_subdirectory(mysql)
endif()

if(BOBUI_FEATURE_sql_odbc)
    add_subdirectory(odbc)
endif()

if(BOBUI_FEATURE_sql_oci)
    add_subdirectory(oci)
endif()

if(BOBUI_FEATURE_sql_db2)
    add_subdirectory(db2)
endif()

if(BOBUI_FEATURE_sql_sqlite)
    add_subdirectory(sqlite)
endif()

if(BOBUI_FEATURE_sql_ibase)
    add_subdirectory(ibase)
endif()

if(BOBUI_FEATURE_sql_mimer)
    add_subdirectory(mimer)
endif()

if(NOT PROJECT_NAME STREQUAL "BobUIBase" AND NOT PROJECT_NAME STREQUAL "BobUI")
    bobui_build_repo_end()
endif()
