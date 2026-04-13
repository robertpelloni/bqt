# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qmetaobject LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

set(tst_qmetaobject_SOURCES
    tst_qmetaobject.cpp
    forwarddeclared.h
    forwarddeclared.cpp
)


#####################################################################
## tst_qmetaobject Test:
#####################################################################

bobui_internal_add_test(tst_qmetaobject
    SOURCES
        ${tst_qmetaobject_SOURCES}
    LIBRARIES
        BobUI::CorePrivate
)

bobui_internal_add_test(tst_qmetaobject_compat
    SOURCES
        ${tst_qmetaobject_SOURCES}
    DEFINES
        USE_COMPAT_Q_ARG=1
    LIBRARIES
        BobUI::CorePrivate
    NO_BATCH
)
