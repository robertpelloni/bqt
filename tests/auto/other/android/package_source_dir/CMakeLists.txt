# Copyright (C) 2025 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_android_package_source_dir LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

# Package name is generated and written to AndroidManifest.xml
bobui_internal_add_test(tst_package_source_dir_not_defined
    SOURCES
        tst_android_package_source_dir.cpp
    DEFINES
        EXPECTED_APP_NAME="tst_package_source_dir_not_defined"
)

# Package name from user-provided AndroidManifest.xml
bobui_internal_add_test(tst_package_source_dir_custom_android_manifest
    SOURCES
        tst_android_package_source_dir.cpp
    DEFINES
        EXPECTED_APP_NAME="tst_package_source_dir_custom_android_manifest_my"
)

if(BOBUI_USE_ANDROID_MODERN_BUNDLE)
    set_target_properties(tst_package_source_dir_custom_android_manifest
        PROPERTIES
            BOBUI_ANDROID_PACKAGE_SOURCE_DIR
            "${CMAKE_CURRENT_SOURCE_DIR}/custom_android_manifest_bundle"
    )
else()
    set_target_properties(tst_package_source_dir_custom_android_manifest
        PROPERTIES
            BOBUI_ANDROID_PACKAGE_SOURCE_DIR
            "${CMAKE_CURRENT_SOURCE_DIR}/custom_android_manifest"
    )
endif()

if(BOBUI_USE_ANDROID_MODERN_BUNDLE)
    # Partial gradle template is only supported by BOBUI_USE_ANDROID_MODERN_BUNDLE
    #
    # Package name from user-provided gradle.build.in
    bobui_internal_add_test(tst_package_source_dir_partial_template
        SOURCES
            tst_android_package_source_dir.cpp
        DEFINES
            EXPECTED_APP_NAME="tst_package_source_dir_partial_template_my"
    )
    set_target_properties(tst_package_source_dir_partial_template
        PROPERTIES
            BOBUI_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/partial_template"
    )
endif()
