# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## Sql Module:
#####################################################################

bobui_internal_add_module(Sql
    PLUGIN_TYPES sqldrivers
    SOURCES
        compat/removed_api.cpp
        kernel/qsqlcachedresult.cpp kernel/qsqlcachedresult_p.h
        kernel/qsqldatabase.cpp kernel/qsqldatabase.h
        kernel/qsqldriver.cpp kernel/qsqldriver.h kernel/qsqldriver_p.h
        kernel/qsqldriverplugin.cpp kernel/qsqldriverplugin.h
        kernel/qsqlerror.cpp kernel/qsqlerror.h
        kernel/qsqlfield.cpp kernel/qsqlfield.h
        kernel/qsqlindex.cpp kernel/qsqlindex.h
        kernel/qsqlnulldriver_p.h
        kernel/qsqlquery.cpp kernel/qsqlquery.h
        kernel/qsqlrecord.cpp kernel/qsqlrecord.h
        kernel/qsqlresult.cpp kernel/qsqlresult.h kernel/qsqlresult_p.h
        kernel/bobuisqlglobal.h kernel/bobuisqlglobal_p.h
    DEFINES
        BOBUI_NO_CAST_FROM_ASCII
        BOBUI_NO_CONTEXTLESS_CONNECT
        BOBUI_NO_FOREACH
        BOBUI_NO_URL_CAST_FROM_STRING
        BOBUI_NO_USING_NAMESPACE
        BOBUI_USE_NODISCARD_FILE_OPEN
    LIBRARIES
        BobUI::CorePrivate
    PUBLIC_LIBRARIES
        BobUI::Core
    PRIVATE_MODULE_INTERFACE
        BobUI::CorePrivate
    NO_PCH_SOURCES
        "compat/removed_api.cpp"
    PRECOMPILED_HEADER
        "../corelib/global/bobui_pch.h"
)

## Scopes:
#####################################################################

bobui_internal_extend_target(Sql CONDITION MSVC AND (TEST_architecture_arch STREQUAL "i386")
    LINK_OPTIONS
        "/BASE:0x62000000"
)

bobui_internal_extend_target(Sql CONDITION BOBUI_FEATURE_sqlmodel
    SOURCES
        models/qsqlquerymodel.cpp models/qsqlquerymodel.h models/qsqlquerymodel_p.h
        models/qsqlrelationaldelegate.cpp models/qsqlrelationaldelegate.h
        models/qsqlrelationaltablemodel.cpp models/qsqlrelationaltablemodel.h
        models/qsqltablemodel.cpp models/qsqltablemodel.h models/qsqltablemodel_p.h
)
bobui_internal_add_docs(Sql
    doc/bobuisql.qdocconf
)

if(BOBUI_BUILD_DOC_SNIPPETS)
    add_subdirectory(doc/snippets/code)
    add_subdirectory(doc/snippets/sqldatabase)
endif()
