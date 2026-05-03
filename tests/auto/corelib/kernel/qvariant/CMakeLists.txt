# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qvariant Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qvariant LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

# Resources:
set(qvariant_resource_files
    "stream/bobui4.9/"
    "stream/bobui5.0/"
)

bobui_internal_add_test(tst_qvariant
    SOURCES
        tst_qvariant.cpp
    INCLUDE_DIRECTORIES
        ../../../other/qvariant_common
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::TestPrivate
    TESTDATA ${qvariant_resource_files}
    BUILTIN_TESTDATA
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_qvariant CONDITION MSVC
    COMPILE_OPTIONS
        /bigobj
)

bobui_internal_extend_target(tst_qvariant CONDITION NOT BOBUI_FEATURE_doubleconversion AND NOT BOBUI_FEATURE_system_doubleconversion
    DEFINES
        BOBUI_NO_DOUBLECONVERSION
)

if(BUILD_SHARED_LIBS AND BOBUI_FEATURE_library)
    bobui_add_library(tst_qvariant_relocatabilitychange MODULE relocatable_change.cpp)
    target_link_libraries(tst_qvariant_relocatabilitychange PRIVATE BobUI6::Core)
    add_dependencies(tst_qvariant tst_qvariant_relocatabilitychange)
endif()
