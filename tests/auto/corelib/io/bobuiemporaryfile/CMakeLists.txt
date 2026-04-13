# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bobuiemporaryfile Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_bobuiemporaryfile LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

# Collect test data
list(APPEND test_data "tst_bobuiemporaryfile.cpp")

bobui_internal_add_test(tst_bobuiemporaryfile
    SOURCES
        tst_bobuiemporaryfile.cpp
    LIBRARIES
        BobUI::TestPrivate
    TESTDATA ${test_data}
)

# Resources:
set(bobuiemporaryfile_resource_files
    "resources/test.txt"
)

bobui_internal_add_resource(tst_bobuiemporaryfile "bobuiemporaryfile"
    PREFIX
        "/"
    FILES
        ${bobuiemporaryfile_resource_files}
)


## Scopes:
#####################################################################

if(ANDROID)
    # Resources:
    set(android_testdata_resource_files
        "resources/test.txt"
        "tst_bobuiemporaryfile.cpp"
    )

    bobui_internal_add_resource(tst_bobuiemporaryfile "android_testdata"
        PREFIX
            "/android_testdata"
        FILES
            ${android_testdata_resource_files}
    )
endif()

bobui_internal_extend_target(tst_bobuiemporaryfile CONDITION WIN32
    LIBRARIES
        shlwapi
)
