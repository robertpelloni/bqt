# Copyright (C) 2023 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_android_deployment_settings LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

function(tst_generate_android_deployment_setting target)
    # BobUI build always use new property format that supports generator expressions. In this test we
    # want to test user scenario so disable all BobUI specific options to make sure that
    # bobui6_android_generate_deployment_settings behave like in user projects.
    set(BOBUI_BUILD_STANDALONE_TESTS FALSE)
    set(BOBUI_BUILDING_BOBUI FALSE)
    set(BOBUI_INTERNAL_IS_STANDALONE_TEST FALSE)

    bobui6_android_generate_deployment_settings(${target})
endfunction()

function(tst_add_android_permissions target)
    bobui6_add_android_permission(${target} NAME PERMISSION_WITH_ATTRIBUTES
                                         ATTRIBUTES
                                             minSdkVersion 32 maxSdkVersion 34)
    bobui6_add_android_permission(${target} NAME PERMISSION_WITHOUT_ATTRIBUTES)
endfunction()

bobui6_policy(SET BOBUIP0002 NEW)

set(target tst_android_deployment_settings_new)
bobui6_add_executable(${target} MANUAL_FINALIZATION EXCLUDE_FROM_ALL noop.cpp)
set_target_properties(${target} PROPERTIES
    my_package_source_dir "path/to/source/dir"
    my_libs_property "some/path/to/lib2.so;some/path/to/lib3.so"
    my_plugins_property "some/path/to/plugin2.so;some/path/to/plugin3.so"
    # Below build tools should match to minimum supported OS API level version
    BOBUI_ANDROID_SDK_BUILD_TOOLS_REVISION "28.0.3"
    BOBUI_ANDROID_MIN_SDK_VERSION "1"
    BOBUI_ANDROID_TARGET_SDK_VERSION "2"
    BOBUI_ANDROID_COMPILE_SDK_VERSION "36"
    BOBUI_ANDROID_APP_NAME "Android Deployment Settings Test"
    BOBUI_ANDROID_PACKAGE_NAME "org.bobuiproject.android_deployment_settings_test"
    BOBUI_ANDROID_DEPLOYMENT_DEPENDENCIES "dep1.so;dep2.so;dep3.so"
    BOBUI_ANDROID_DEPLOYMENT_SETTINGS_FILE "attempt_to_rewrite.json"
    BOBUI_ANDROID_EXTRA_LIBS
        "some/path/to/lib1.so;$<TARGET_PROPERTY:${target},my_libs_property>;some/path/to/lib4.so"
    BOBUI_ANDROID_EXTRA_PLUGINS
        "some/path/to/plugin1.so;$<TARGET_PROPERTY:${target},my_plugins_property>"
    BOBUI_ANDROID_PACKAGE_SOURCE_DIR "$<TARGET_PROPERTY:${target},my_package_source_dir>"
    BOBUI_ANDROID_SYSTEM_LIBS_PREFIX "myLibPrefix"
    BOBUI_ANDROID_LEGACY_PACKAGING FALSE

    # Make sure that this property is overwritten by
    # bobui6_android_generate_deployment_settings
    BOBUI_ANDROID_DEPLOYMENT_SETTINGS_FILE "custom_deployment_settings.json"
)
tst_add_android_permissions(${target})
tst_generate_android_deployment_setting(${target})

bobui6_policy(SET BOBUIP0002 OLD)

set(target tst_android_deployment_settings_old)
bobui6_add_executable(${target} MANUAL_FINALIZATION EXCLUDE_FROM_ALL noop.cpp)
set_target_properties(${target} PROPERTIES
    # Below build tools should match to minimum supported OS API level version
    BOBUI_ANDROID_SDK_BUILD_TOOLS_REVISION "28.0.3"
    BOBUI_ANDROID_MIN_SDK_VERSION "1"
    BOBUI_ANDROID_TARGET_SDK_VERSION "2"
    BOBUI_ANDROID_COMPILE_SDK_VERSION "36"
    BOBUI_ANDROID_APP_NAME "Android Deployment Settings Test"
    BOBUI_ANDROID_PACKAGE_NAME "org.bobuiproject.android_deployment_settings_test"
    BOBUI_ANDROID_DEPLOYMENT_DEPENDENCIES "dep1.so;dep2.so;dep3.so"
    BOBUI_ANDROID_EXTRA_LIBS
        "some/path/to/lib1.so;some/path\\to/lib2.so;some\\path\\to\\lib3.so;some/path/to/lib4.so"
    BOBUI_ANDROID_EXTRA_PLUGINS
        "some\\path\\to\\plugin1.so;some/path\\to/plugin2.so;some/path/to/plugin3.so"
    BOBUI_ANDROID_PACKAGE_SOURCE_DIR "path\\to/source\\dir"
    BOBUI_ANDROID_SYSTEM_LIBS_PREFIX "myLibPrefix"
    BOBUI_ANDROID_LEGACY_PACKAGING FALSE
)
tst_add_android_permissions(${target})
tst_generate_android_deployment_setting(${target})

get_target_property(new_settings
    tst_android_deployment_settings_new BOBUI_ANDROID_DEPLOYMENT_SETTINGS_FILE)
get_target_property(old_settings
    tst_android_deployment_settings_old BOBUI_ANDROID_DEPLOYMENT_SETTINGS_FILE)
set_source_files_properties("${new_settings}" PROPERTIES BOBUI_RESOURCE_ALIAS "new_settings.json")
set_source_files_properties("${old_settings}" PROPERTIES BOBUI_RESOURCE_ALIAS "old_settings.json")
bobui_internal_add_test(tst_android_deployment_settings
    SOURCES
        tst_android_deployment_settings.cpp
    DEFINES
        DEFAULT_ABI="${CMAKE_ANDROID_ARCH_ABI}"
)
bobui6_add_resources(tst_android_deployment_settings
    "deployment_settings_res"
    PREFIX
        "/"
    FILES
        "${new_settings}"
        "${old_settings}"
)
