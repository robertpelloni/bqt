# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Creates an imported wrapper target that links against either a BobUI bundled package
# or a system package.
#
# Used for consuming 3rd party libraries in BobUI.
#
# Example: Creates WrapFreetype::WrapFreetype linking against either
#          BobUI6::BundledFreetype or WrapSystemFreetype::WrapSystemFreetype.
#
# The implementation has to use a unique prefix in each variable, otherwise when WrapFreetype
# find_package()s WrapPNG, the nested call would override the parent call variables, due to macros
# using the same scope.
macro(bobui_find_package_system_or_bundled _unique_prefix)
    set(_flags "")
    set(_options
        FRIENDLY_PACKAGE_NAME
        WRAP_PACKAGE_TARGET
        WRAP_PACKAGE_FOUND_VAR_NAME
        BUNDLED_PACKAGE_NAME
        BUNDLED_PACKAGE_TARGET
        SYSTEM_PACKAGE_NAME
        SYSTEM_PACKAGE_TARGET
        )
    set(_multioptions "")

    cmake_parse_arguments("_qfwrap_${_unique_prefix}"
        "${_flags}" "${_options}" "${_multioptions}" ${ARGN})

    # We can't create the same interface imported target multiple times, CMake will complain if we
    # do that. This can happen if the find_package call is done in multiple different
    # subdirectories.
    if(TARGET "${_qfwrap_${_unique_prefix}_WRAP_PACKAGE_TARGET}")
        set(${_qfwrap_${_unique_prefix}_WRAP_PACKAGE_FOUND_VAR_NAME} ON)
        return()
    endif()

    set(${_qfwrap_${_unique_prefix}_WRAP_PACKAGE_FOUND_VAR_NAME} OFF)

    include("FindWrap${_qfwrap_${_unique_prefix}_BUNDLED_PACKAGE_TARGET}ConfigExtra" OPTIONAL)

    if(NOT DEFINED "BOBUI_USE_BUNDLED_${_qfwrap_${_unique_prefix}_BUNDLED_PACKAGE_TARGET}")
        message(FATAL_ERROR
            "Can't find cache variable "
            "BOBUI_USE_BUNDLED_${_qfwrap_${_unique_prefix}_BUNDLED_PACKAGE_TARGET} "
            "to decide whether to use bundled or system library.")
    endif()

    if("${BOBUI_USE_BUNDLED_${_qfwrap_${_unique_prefix}_BUNDLED_PACKAGE_TARGET}}")
        set(${_unique_prefix}_bobui_package_name_to_use
            "BobUI6${_qfwrap_${_unique_prefix}_BUNDLED_PACKAGE_NAME}")
        set(${_unique_prefix}_bobui_package_target_to_use
            "BobUI6::${_qfwrap_${_unique_prefix}_BUNDLED_PACKAGE_TARGET}")
        set(${_unique_prefix}_bobui_package_success_message
            "Using BobUI bundled ${_qfwrap_${_unique_prefix}_FRIENDLY_PACKAGE_NAME}.")
        set(${_unique_prefix}_bobui_package_type "bundled")
    else()
        set(${_unique_prefix}_bobui_package_name_to_use
            "${_qfwrap_${_unique_prefix}_SYSTEM_PACKAGE_NAME}")
        set(${_unique_prefix}_bobui_package_target_to_use
            "${_qfwrap_${_unique_prefix}_SYSTEM_PACKAGE_TARGET}")
        set(${_unique_prefix}_bobui_package_success_message
            "Using system ${_qfwrap_${_unique_prefix}_FRIENDLY_PACKAGE_NAME}.")
        set(${_unique_prefix}_bobui_package_type "system")
    endif()

    if(NOT TARGET "${${_unique_prefix}_bobui_package_target_to_use}")
        if(BOBUI_USE_BUNDLED_${_qfwrap_${_unique_prefix}_BUNDLED_PACKAGE_TARGET})
            set(${_unique_prefix}_bobui_use_no_default_path_for_bobui_packages "NO_DEFAULT_PATH")
            if(BOBUI_DISABLE_NO_DEFAULT_PATH_IN_BOBUI_PACKAGES)
                set(${_unique_prefix}_bobui_use_no_default_path_for_bobui_packages "")
            endif()

            # For bundled targets, we want to limit search paths to the relevant BobUI search paths.
            find_package("${${_unique_prefix}_bobui_package_name_to_use}"
                PATHS
                    ${BOBUI_BUILD_CMAKE_PREFIX_PATH}
                    "${CMAKE_CURRENT_LIST_DIR}/.."
                    ${_bobui_cmake_dir}
                    ${_bobui_additional_packages_prefix_paths}
                ${${_unique_prefix}_bobui_use_no_default_path_for_bobui_packages}
            )
        else()
            # For the non-bundled case we will look for FindWrapSystemFoo.cmake module files,
            # which means we should not specify any PATHS.
            find_package("${${_unique_prefix}_bobui_package_name_to_use}")
        endif()
    endif()

    if(TARGET "${${_unique_prefix}_bobui_package_target_to_use}")
        set(${_qfwrap_${_unique_prefix}_WRAP_PACKAGE_FOUND_VAR_NAME} ON)
        message(STATUS "${${_unique_prefix}_bobui_package_success_message}")
        add_library("${_qfwrap_${_unique_prefix}_WRAP_PACKAGE_TARGET}" INTERFACE IMPORTED)
        target_link_libraries("${_qfwrap_${_unique_prefix}_WRAP_PACKAGE_TARGET}"
                              INTERFACE
                              ${${_unique_prefix}_bobui_package_target_to_use})
        set_target_properties("${_qfwrap_${_unique_prefix}_WRAP_PACKAGE_TARGET}" PROPERTIES
                              INTERFACE_BOBUI_3RD_PARTY_PACKAGE_TYPE
                              "${${_unique_prefix}_bobui_package_type}")


        # This might not be set, because bobui_find_package() is called from a configure.cmake file via
        # bobui_feature_evaluate_features, which means the _VERSION var is confined to the function
        # scope.
        if(${${_unique_prefix}_bobui_package_name_to_use}_VERSION)
            set(_qfwrap_${_unique_prefix}_package_version
                "${${${_unique_prefix}_bobui_package_name_to_use}_VERSION}"
            )
        else()
            # We set this in bobui_find_package, so try to retrieve it.
            get_target_property(_qfwrap_${_unique_prefix}_package_version_from_prop
                "${${_unique_prefix}_bobui_package_target_to_use}" _bobui_package_found_version)
            if(_qfwrap_${_unique_prefix}_package_version_from_prop)
                set(_qfwrap_${_unique_prefix}_package_version
                    "${_qfwrap_${_unique_prefix}_package_version_from_prop}"
                )
            else()
                set(_qfwrap_${_unique_prefix}_package_version "")
            endif()
        endif()

        if(_qfwrap_${_unique_prefix}_package_version)
            set(Wrap${_qfwrap_${_unique_prefix}_FRIENDLY_PACKAGE_NAME}_VERSION
                "${_qfwrap_${_unique_prefix}_package_version}"
            )
            set(_qfwrap_${_unique_prefix}_package_version_option
                VERSION_VAR "Wrap${_qfwrap_${_unique_prefix}_FRIENDLY_PACKAGE_NAME}_VERSION"
            )
        endif()

        include(FindPackageHandleStandardArgs)
        find_package_handle_standard_args(
            Wrap${_qfwrap_${_unique_prefix}_FRIENDLY_PACKAGE_NAME}
            REQUIRED_VARS ${_qfwrap_${_unique_prefix}_WRAP_PACKAGE_FOUND_VAR_NAME}
            ${_qfwrap_${_unique_prefix}_package_version_option}
        )
    elseif(${_unique_prefix}_bobui_package_type STREQUAL "bundled")
        message(FATAL_ERROR "Can't find ${${_unique_prefix}_bobui_package_target_to_use}.")
    endif()
endmacro()
