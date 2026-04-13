# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qfileinfo Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qfileinfo LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qfileinfo
    SOURCES
        tst_qfileinfo.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::TestPrivate
)

# Resources:
set(qfileinfo_resource_files
    "resources/"
)

bobui_internal_add_resource(tst_qfileinfo "qfileinfo"
    PREFIX
        "/tst_qfileinfo/"
    FILES
        ${qfileinfo_resource_files}
)
set(testdata_resource_files
    "resources/file1"
    "resources/file1.ext1"
    "resources/file1.ext1.ext2"
    "tst_qfileinfo.cpp"
)

bobui_internal_add_resource(tst_qfileinfo "testdata"
    PREFIX
        "/testdata"
    FILES
        ${testdata_resource_files}
)


## Scopes:
#####################################################################

bobui_internal_extend_target(tst_qfileinfo CONDITION WIN32
    LIBRARIES
        advapi32
        netapi32
)

if (APPLE)
    enable_language(OBJCXX)
    set_source_files_properties(tst_qfileinfo.cpp PROPERTIES LANGUAGE OBJCXX)
endif()
