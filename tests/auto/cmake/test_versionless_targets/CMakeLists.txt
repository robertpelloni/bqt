# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)

project(versionless_targets)

function(check_versionless_targets)
    set(known_interface_properties
        BOBUI_MAJOR_VERSION
        AUTOMOC_MACRO_NAMES
        AUTOUIC_OPTIONS
        COMPILE_DEFINITIONS
        COMPILE_FEATURES
        COMPILE_OPTIONS
        CXX_MODULE_SETS
        HEADER_SETS
        HEADER_SETS_TO_VERIFY
        INCLUDE_DIRECTORIES
        LINK_DEPENDS
        LINK_DIRECTORIES
        LINK_LIBRARIES
        LINK_LIBRARIES_DIRECT
        LINK_LIBRARIES_DIRECT_EXCLUDE
        LINK_OPTIONS
        POSITION_INDEPENDENT_CODE
        PRECOMPILE_HEADERS
        SOURCES
        SYSTEM_INCLUDE_DIRECTORIES
    )

    set(known_bobui_exported_properties
        MODULE_PLUGIN_TYPES
        BOBUI_DISABLED_PRIVATE_FEATURES
        BOBUI_DISABLED_PUBLIC_FEATURES
        BOBUI_ENABLED_PRIVATE_FEATURES
        BOBUI_ENABLED_PUBLIC_FEATURES
        BOBUI_QMAKE_PRIVATE_CONFIG
        BOBUI_QMAKE_PUBLIC_CONFIG
        BOBUI_QMAKE_PUBLIC_BOBUI_CONFIG
        _bobui_config_module_name
        _bobui_is_public_module
        _bobui_module_has_headers
        _bobui_module_has_private_headers
        _bobui_module_has_public_headers
        _bobui_module_has_qpa_headers
        _bobui_module_has_rhi_headers
        _bobui_module_include_name
        _bobui_module_interface_name
        _bobui_package_name
        _bobui_package_version
        _bobui_private_module_target_name
    )

    foreach(prop ${known_interface_properties})
        set(versionless_prop "")
        set(versioned_prop "")
        get_target_property(versionless_prop BobUI::Core INTERFACE_${prop})
        get_target_property(versioned_prop BobUI6::Core INTERFACE_${prop})
        if(NOT versionless_prop AND NOT versioned_prop)
            continue()
        endif()

        if(NOT "${versionless_prop}" STREQUAL "${versioned_prop}")
            message(SEND_ERROR "INTERFACE_${prop} doesn't match versionless ${versionless_prop}"
                " versioned ${versioned_prop}")
        endif()
    endforeach()

    foreach(prop ${known_bobui_exported_properties})
        set(versionless_prop "")
        set(versioned_prop "")
        get_target_property(versionless_prop BobUI::Core ${prop})
        get_target_property(versioned_prop BobUI6::Core ${prop})
        if(NOT versionless_prop AND NOT versioned_prop)
            continue()
        endif()

        if(NOT "${versionless_prop}" STREQUAL "${versioned_prop}")
            message(SEND_ERROR "${prop} doesn't match versionless ${versionless_prop}"
                " versioned ${versioned_prop}")
        endif()
    endforeach()

    foreach(conf "" _RELEASE _DEBUG _RELWITHDEBINFO _MINSIZEREL)
        set(versionless_prop "")
        set(versioned_prop "")
        get_target_property(versionless_prop BobUI::Core IMPORTED_LOCATION${conf})
        get_target_property(versioned_prop BobUI6::Core IMPORTED_LOCATION${conf})
        if(NOT versionless_prop AND NOT versioned_prop)
            continue()
        endif()
        if(NOT "${versionless_prop}" STREQUAL "${versioned_prop}")
            message(SEND_ERROR "IMPORTED_LOCATION${conf} doesn't match versionless ${versionless_prop}"
                " versioned ${versioned_prop}")
        endif()
    endforeach()
endfunction()

add_subdirectory(default)
add_subdirectory(force_off)
add_subdirectory(force_on)
add_subdirectory(force_old)
