# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bobuiranslator Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_bobuiranslator LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

if (NOT BOBUI_FEATURE_thread)
    return()
endif()

bobui_internal_add_test(tst_bobuiranslator
    SOURCES
        tst_bobuiranslator.cpp
    LIBRARIES
        BobUI::CorePrivate
)

# Resources:
set(bobuiranslator_resource_files
    "dependencies_la.qm"
    "hellotr_empty.qm"
    "hellotr_la.qm"
    "msgfmt_from_po.qm"
)

bobui_internal_add_resource(tst_bobuiranslator "bobuiranslator"
    PREFIX
        "/tst_bobuiranslator"
    FILES
        ${bobuiranslator_resource_files}
)

## Scopes:
#####################################################################

if(ANDROID)
    # Resources:
    set(android_testdata_resource_files
        "dependencies_la.qm"
        "hellotr_empty.qm"
        "hellotr_la.qm"
        "msgfmt_from_po.qm"
    )

    bobui_internal_add_resource(tst_bobuiranslator "android_testdata"
        PREFIX
            "/android_testdata"
        FILES
            ${android_testdata_resource_files}
    )
endif()
