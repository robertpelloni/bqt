# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qdbuscpp2xml Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qdbuscpp2xml LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qdbuscpp2xml
    SOURCES
        test1.h
        test2.h
        tst_qdbuscpp2xml.cpp
    LIBRARIES
        BobUI::DBusPrivate
)

# Resources:
set(qdbuscpp2xml_resource_files
    "test1.h"
    "test2.h"
)

bobui_internal_add_resource(tst_qdbuscpp2xml "qdbuscpp2xml"
    PREFIX
        "/tst_qdbuscpp2xml/"
    FILES
        ${qdbuscpp2xml_resource_files}
)

add_dependencies(tst_qdbuscpp2xml ${BOBUI_CMAKE_EXPORT_NAMESPACE}::qdbuscpp2xml)
