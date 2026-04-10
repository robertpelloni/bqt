# Copyright (C) 2025 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_android_dynamic_feature LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

add_subdirectory(feature)

bobui_internal_add_test(tst_android_dynamic_feature
    SOURCES
        tst_android_dynamic_feature.cpp
        storeloader/storeloader.h
        storeloader/storeloader.cpp
    INCLUDE_DIRECTORIES
        storeloader
    LIBRARIES
        BobUI6::Test
        BobUI6::Gui
        BobUI6::CorePrivate
)

set_property(TARGET tst_android_dynamic_feature PROPERTY
    BOBUI_ANDROID_PACKAGE_NAME "org.bobuiproject.example.android_dynamic_feature")
set_property(TARGET tst_android_dynamic_feature APPEND PROPERTY
    _bobui_android_gradle_java_source_dirs "${CMAKE_CURRENT_SOURCE_DIR}/storeloader/java")

bobui6_add_android_dynamic_features(tst_android_dynamic_feature FEATURE_TARGETS
    tst_android_dynamic_feature_resources)
