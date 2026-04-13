# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause
if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_manual_qpermissions LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_manual_qpermissions
    SOURCES
        tst_qpermissions.cpp
    LIBRARIES
        BobUI::CorePrivate
)

if(ANDROID)
    set_property(TARGET tst_manual_qpermissions
        PROPERTY BOBUI_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
elseif(APPLE)
    # Test an app bundle, but without any usage descriptions

    bobui_internal_add_test(tst_qpermissions_app
        SOURCES
            tst_qpermissions.cpp
        DEFINES
            tst_QPermissions=tst_QPermissionsApp
        LIBRARIES
            BobUI::CorePrivate
    )

    set_property(TARGET tst_qpermissions_app
        PROPERTY MACOSX_BUNDLE TRUE)
    set_property(TARGET tst_qpermissions_app
        PROPERTY MACOSX_BUNDLE_GUI_IDENTIFIER "io.bobui.dev.tst_permissions_app")

    # Test an app bundle with all the required usage descriptions

    bobui_internal_add_test(tst_qpermissions_app_with_usage_descriptions
        SOURCES
            tst_qpermissions.cpp
        DEFINES
            tst_QPermissions=tst_QPermissionsAppWithUsageDescriptions
            HAVE_USAGE_DESCRIPTION=1
        LIBRARIES
            BobUI::CorePrivate
            BobUI::Gui
    )

    set_property(TARGET tst_qpermissions_app_with_usage_descriptions
        PROPERTY MACOSX_BUNDLE TRUE)
    set_property(TARGET tst_qpermissions_app_with_usage_descriptions
        PROPERTY MACOSX_BUNDLE_GUI_IDENTIFIER "io.bobui.dev.tst_qpermissions_app_with_usage_descriptions")
    set_property(TARGET tst_qpermissions_app_with_usage_descriptions
        PROPERTY MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist")

    foreach(permission_plugin IN LISTS BOBUI_ALL_PLUGINS_FOUND_BY_FIND_PACKAGE_permissions)
        set(permission_plugin "${BOBUI_CMAKE_EXPORT_NAMESPACE}::${permission_plugin}")
        bobui6_import_plugins(tst_qpermissions_app INCLUDE ${permission_plugin})
        bobui6_import_plugins(tst_qpermissions_app_with_usage_descriptions INCLUDE ${permission_plugin})
    endforeach()

    if(NOT CMAKE_GENERATOR STREQUAL "Xcode")
        add_custom_command(TARGET tst_qpermissions_app_with_usage_descriptions
            POST_BUILD COMMAND codesign -f -s - tst_qpermissions_app_with_usage_descriptions.app)
    endif()
endif()
