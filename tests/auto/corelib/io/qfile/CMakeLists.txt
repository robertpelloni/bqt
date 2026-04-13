# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qfile Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qfile LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

# Collect test data
list(APPEND test_data "dosfile.txt")
list(APPEND test_data "noendofline.txt")
list(APPEND test_data "testfile.txt")
list(APPEND test_data "testlog.txt")
list(APPEND test_data "two.dots.file")
list(APPEND test_data "tst_qfile.cpp")
list(APPEND test_data "forCopying.txt")
list(APPEND test_data "forRenaming.txt")
list(APPEND test_data "resources/file1.ext1")

bobui_internal_add_test(tst_qfile
    SOURCES
        tst_qfile.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::TestPrivate
    TESTDATA ${test_data}
)

# Resources:
set(qfile_resource_files
    "resources/"
)

bobui_internal_add_resource(tst_qfile "qfile"
    PREFIX
        "/tst_qfile/"
    FILES
        ${qfile_resource_files}
)
set(rename-fallback_resource_files
    "rename-fallback.qrc"
)

bobui_internal_add_resource(tst_qfile "rename-fallback"
    PREFIX
        "/"
    FILES
        ${rename-fallback_resource_files}
)
set(copy-fallback_resource_files
    "copy-fallback.qrc"
)

bobui_internal_add_resource(tst_qfile "copy-fallback"
    PREFIX
        "/"
    FILES
        ${copy-fallback_resource_files}
)


## Scopes:
#####################################################################

bobui_internal_extend_target(tst_qfile CONDITION TARGET BobUI::Network
    LIBRARIES
        BobUI::Network
)

bobui_internal_extend_target(tst_qfile CONDITION NOT TARGET BobUI::Network
    DEFINES
        BOBUI_NO_NETWORK
)

bobui_internal_extend_target(tst_qfile CONDITION CONFIG___contains___builtin_testdata
    DEFINES
        BUILTIN_TESTDATA
)

bobui_internal_extend_target(tst_qfile CONDITION WIN32
    LIBRARIES
        ole32
        uuid
)
add_subdirectory(stdinprocess)
if(BOBUI_FEATURE_process)
    add_dependencies(tst_qfile stdinprocess_helper)
endif()
