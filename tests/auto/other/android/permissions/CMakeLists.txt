# Copyright (C) 2025 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_android_permissions LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_android_permissions
    SOURCES
        tst_android_permissions.cpp
)

bobui_add_android_permission(tst_android_permissions
    NAME android.permission.ACCESS_COARSE_LOCATION
)

bobui_add_android_permission(tst_android_permissions
    NAME android.permission.ACCESS_FINE_LOCATION
    ATTRIBUTES
        maxSdkVersion 31
)
