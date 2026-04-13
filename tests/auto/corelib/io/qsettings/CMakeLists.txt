# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qsettings Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qsettings LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

# Test:
set(qsettings_resource_files
    "bom.ini"
    "resourcefile.ini"
    "resourcefile2.ini"
    "resourcefile3.ini"
    "resourcefile6.plist"
    "withcomments.ini"
    "float.ini"
    "bobui5settings.ini"
    "utf8settings.ini"
    "incorrectsection.ini"
)

bobui_internal_add_test(tst_qsettings
    SOURCES
        tst_qsettings.cpp
    INCLUDE_DIRECTORIES
        ../../kernel/qmetatype
    NO_BATCH # BOBUIBUG-121815
    DEFINES
        BOBUIEST_THROW_ON_FAIL
        BOBUIEST_THROW_ON_SKIP
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Gui
    TESTDATA ${qsettings_resource_files}
    BUILTIN_TESTDATA
)


## Scopes:
#####################################################################

bobui_internal_extend_target(tst_qsettings CONDITION MSVC
    LIBRARIES
        advapi32
)

bobui_internal_extend_target(tst_qsettings CONDITION APPLE
    LIBRARIES
        ${FWCoreFoundation}
)
