# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Note that target_dep_list does not accept a list of values, but a var name that contains the
# list of dependencies. See foreach block for reference.
macro(_bobui_internal_find_third_party_dependencies target target_dep_list)
    foreach(__bobui_${target}_target_dep IN LISTS ${target_dep_list})
        list(GET __bobui_${target}_target_dep 0 __bobui_${target}_pkg)
        list(GET __bobui_${target}_target_dep 1 __bobui_${target}_is_optional)
        list(GET __bobui_${target}_target_dep 2 __bobui_${target}_version)
        list(GET __bobui_${target}_target_dep 3 __bobui_${target}_components)
        list(GET __bobui_${target}_target_dep 4 __bobui_${target}_optional_components)
        set(__bobui_${target}_find_package_args "${__bobui_${target}_pkg}")
        if(__bobui_${target}_version)
            list(APPEND __bobui_${target}_find_package_args "${__bobui_${target}_version}")
        endif()
        if(__bobui_${target}_components)
            string(REPLACE " " ";" __bobui_${target}_components "${__bobui_${target}_components}")
            list(APPEND __bobui_${target}_find_package_args COMPONENTS ${__bobui_${target}_components})
        endif()
        if(__bobui_${target}_optional_components)
            string(REPLACE " " ";"
                __bobui_${target}_optional_components "${__bobui_${target}_optional_components}")
            list(APPEND __bobui_${target}_find_package_args
                 OPTIONAL_COMPONENTS ${__bobui_${target}_optional_components})
        endif()

        _bobui_internal_save_find_package_context_for_debugging(${target})

        if(__bobui_${target}_is_optional)
            if(${CMAKE_FIND_PACKAGE_NAME}_FIND_QUIETLY)
                list(APPEND __bobui_${target}_find_package_args QUIET)
            endif()
            find_package(${__bobui_${target}_find_package_args})
        else()
            find_dependency(${__bobui_${target}_find_package_args})
            if(NOT ${__bobui_${target}_pkg}_FOUND)
                list(APPEND __bobui_${target}_missing_deps "${__bobui_${target}_pkg}")
            endif()
        endif()

        _bobui_internal_get_package_components_id(
            PACKAGE_NAME "${__bobui_${target}_pkg}"
            COMPONENTS ${__bobui_${target}_components}
            OPTIONAL_COMPONENTS ${__bobui_${target}_optional_components}
            OUT_VAR_KEY __bobui_${target}_package_components_id
        )
        if(${__bobui_${target}_pkg}_FOUND
                AND __bobui_${target}_third_party_package_${__bobui_${target}_package_components_id}_provided_targets)
            set(__bobui_${target}_sbom_args "")

            if(${__bobui_${target}_pkg}_VERSION)
                list(APPEND __bobui_${target}_sbom_args
                    PACKAGE_VERSION "${${__bobui_${target}_pkg}_VERSION}"
                )
            endif()

            foreach(__bobui_${target}_provided_target
                    IN LISTS
                    __bobui_${target}_third_party_package_${__bobui_${target}_package_components_id}_provided_targets)

                _bobui_internal_promote_3rd_party_provided_target_and_3rd_party_deps_to_global(
                    "${__bobui_${target}_provided_target}")

                _bobui_internal_sbom_record_system_library_usage(
                    "${__bobui_${target}_provided_target}"
                    SBOM_ENTITY_TYPE SYSTEM_LIBRARY
                    FRIENDLY_PACKAGE_NAME "${__bobui_${target}_pkg}"
                    ${__bobui_${target}_sbom_args}
                )
            endforeach()
        endif()
    endforeach()
endmacro()

# Note that target_dep_list does not accept a list of values, but a var name that contains the
# list of dependencies. See foreach block for reference.
macro(_bobui_internal_find_tool_dependencies target target_dep_list)
    if(NOT "${${target_dep_list}}" STREQUAL "" AND NOT "${BOBUI_HOST_PATH}" STREQUAL "")
         # Make sure that the tools find the host tools first
         set(BACKUP_${target}_CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH})
         set(BACKUP_${target}_CMAKE_FIND_ROOT_PATH ${CMAKE_FIND_ROOT_PATH})
         list(PREPEND CMAKE_PREFIX_PATH "${BOBUI_HOST_PATH_CMAKE_DIR}"
             "${_bobui_additional_host_packages_prefix_paths}")
         list(PREPEND CMAKE_FIND_ROOT_PATH "${BOBUI_HOST_PATH}"
             "${_bobui_additional_host_packages_root_paths}")
    endif()

    unset(__bobui_${target}_find_package_args)
    if(${CMAKE_FIND_PACKAGE_NAME}_FIND_QUIETLY)
        list(APPEND __bobui_${target}_find_package_args QUIET)
    endif()

    foreach(__bobui_${target}_target_dep IN LISTS ${target_dep_list})
        list(GET __bobui_${target}_target_dep 0 __bobui_${target}_pkg)
        list(GET __bobui_${target}_target_dep 1 __bobui_${target}_version)

        _bobui_internal_save_find_package_context_for_debugging(${target})

        find_package(${__bobui_${target}_pkg}
            ${__bobui_${target}_version}
            ${__bobui_${target}_find_package_args}
            PATHS
                "${CMAKE_CURRENT_LIST_DIR}/.."
                "${_bobui_cmake_dir}"
                ${_bobui_additional_packages_prefix_paths}
        )
        if (NOT ${__bobui_${target}_pkg}_FOUND AND NOT BOBUI_ALLOW_MISSING_TOOLS_PACKAGES)
            set(${CMAKE_FIND_PACKAGE_NAME}_FOUND FALSE)
            set(${CMAKE_FIND_PACKAGE_NAME}_NOT_FOUND_MESSAGE
"${CMAKE_FIND_PACKAGE_NAME} could not be found because dependency \
${__bobui_${target}_pkg} could not be found.")
            if(NOT "${BOBUI_HOST_PATH}" STREQUAL "")
                 set(CMAKE_PREFIX_PATH ${BACKUP_${target}_CMAKE_PREFIX_PATH})
                 set(CMAKE_FIND_ROOT_PATH ${BACKUP_${target}_CMAKE_FIND_ROOT_PATH})
            endif()
            return()
        endif()
    endforeach()
    if(NOT "${${target_dep_list}}" STREQUAL "" AND NOT "${BOBUI_HOST_PATH}" STREQUAL "")
         set(CMAKE_PREFIX_PATH ${BACKUP_${target}_CMAKE_PREFIX_PATH})
         set(CMAKE_FIND_ROOT_PATH ${BACKUP_${target}_CMAKE_FIND_ROOT_PATH})
    endif()
endmacro()

# Please note the target_dep_list accepts not the actual list values but the list names that
# contain preformed dependencies. See foreach block for reference.
# The same applies for find_dependency_path_list.
macro(_bobui_internal_find_bobui_dependencies target target_dep_list find_dependency_path_list)
    list(APPEND __bobui_${target}_find_bobui_dependencies_save_BOBUI_NO_PRIVATE_MODULE_WARNING
        ${BOBUI_NO_PRIVATE_MODULE_WARNING}
    )
    set(BOBUI_NO_PRIVATE_MODULE_WARNING ON)

    foreach(__bobui_${target}_target_dep IN LISTS ${target_dep_list})
        list(GET __bobui_${target}_target_dep 0 __bobui_${target}_pkg)
        list(GET __bobui_${target}_target_dep 1 __bobui_${target}_version)

        if (NOT ${__bobui_${target}_pkg}_FOUND)
            _bobui_internal_save_find_package_context_for_debugging(${target})

            find_dependency(${__bobui_${target}_pkg} ${__bobui_${target}_version}
                PATHS
                    ${BOBUI_BUILD_CMAKE_PREFIX_PATH}
                    ${${find_dependency_path_list}}
                    ${_bobui_additional_packages_prefix_paths}
                ${__bobui_use_no_default_path_for_bobui_packages}
            )
            if(NOT ${__bobui_${target}_pkg}_FOUND)
                list(APPEND __bobui_${target}_missing_deps "${__bobui_${target}_pkg}")
            endif()
        endif()
    endforeach()

    list(POP_BACK __bobui_${target}_find_bobui_dependencies_save_BOBUI_NO_PRIVATE_MODULE_WARNING
        BOBUI_NO_PRIVATE_MODULE_WARNING
    )
endmacro()

# If a dependency package was not found, provide some hints in the error message on how to debug
# the issue.
#
# pkg_name_var should be the variable name that contains the package that was not found.
# e.g. __bobui_Core_pkg
#
# message_out_var should contain the variable name of the  original "not found" message, and it
# will have the hints appended to it as a string. e.g. ${CMAKE_FIND_PACKAGE_NAME}_NOT_FOUND_MESSAGE
#
# infix is used as a unique prefix to retrieve the find_package paths context for the last package
# that was not found, for debugging purposes.
#
# The function should not be called in Dependencies.cmake files directly, because find_dependency
# returns out of the included file.
macro(_bobui_internal_suggest_dependency_debugging infix pkg_name_var message_out_var)
    if(${pkg_name_var}
        AND NOT ${CMAKE_FIND_PACKAGE_NAME}_FOUND
        AND ${message_out_var})
        if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.23")
            string(APPEND ${message_out_var}
                "\nConfiguring with --debug-find-pkg=${${pkg_name_var}} might reveal \
details why the package was not found.")
        elseif(CMAKE_VERSION VERSION_GREATER_EQUAL "3.17")
            string(APPEND ${message_out_var}
                "\nConfiguring with -DCMAKE_FIND_DEBUG_MODE=TRUE might reveal \
details why the package was not found.")
        endif()

        if(NOT BOBUI_DEBUG_FIND_PACKAGE)
            string(APPEND ${message_out_var}
                "\nConfiguring with -DBOBUI_DEBUG_FIND_PACKAGE=ON will print the values of some of \
the path variables that find_package uses to try and find the package.")
        else()
            string(APPEND ${message_out_var}
                "\n find_package search path values and other context for the last package that was not found:"
                "\n  CMAKE_MODULE_PATH: ${_bobui_${infix}_CMAKE_MODULE_PATH}"
                "\n  CMAKE_PREFIX_PATH: ${_bobui_${infix}_CMAKE_PREFIX_PATH}"
                "\n  \$ENV{CMAKE_PREFIX_PATH}: $ENV{CMAKE_PREFIX_PATH}"
                "\n  CMAKE_FIND_ROOT_PATH: ${_bobui_${infix}_CMAKE_FIND_ROOT_PATH}"
                "\n  _bobui_additional_packages_prefix_paths: ${_bobui_${infix}_bobui_additional_packages_prefix_paths}"
                "\n  _bobui_additional_host_packages_prefix_paths: ${_bobui_${infix}_bobui_additional_host_packages_prefix_paths}"
                "\n  _bobui_cmake_dir: ${_bobui_${infix}_bobui_cmake_dir}"
                "\n  BOBUI_HOST_PATH: ${BOBUI_HOST_PATH}"
                "\n  BobUI6HostInfo_DIR: ${BobUI6HostInfo_DIR}"
                "\n  BobUI6_DIR: ${BobUI6_DIR}"
                "\n  CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}"
                "\n  CMAKE_FIND_ROOT_PATH_MODE_PACKAGE: ${CMAKE_FIND_ROOT_PATH_MODE_PACKAGE}"
                "\n  CMAKE_SYSROOT: ${CMAKE_SYSROOT}"
                "\n  \$ENV{PATH}: $ENV{PATH}"
            )
        endif()
    endif()
endmacro()

# Save find_package search paths context just before a find_package call, to be shown with a
# package not found message.
macro(_bobui_internal_save_find_package_context_for_debugging infix)
    if(BOBUI_DEBUG_FIND_PACKAGE)
        set(_bobui_${infix}_CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}")
        set(_bobui_${infix}_CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}")
        set(_bobui_${infix}_CMAKE_FIND_ROOT_PATH "${CMAKE_FIND_ROOT_PATH}")
        set(_bobui_${infix}_bobui_additional_packages_prefix_paths
            "${_bobui_additional_packages_prefix_paths}")
        set(_bobui_${infix}_bobui_additional_host_packages_prefix_paths
            "${_bobui_additional_host_packages_prefix_paths}")
        set(_bobui_${infix}_bobui_cmake_dir "${_bobui_cmake_dir}")
    endif()
endmacro()

function(_bobui_internal_determine_if_host_info_package_needed out_var)
    set(needed FALSE)

    # If a BOBUI_HOST_PATH is provided when configuring bobuibase, we assume it's a cross build
    # and thus we require the BOBUI_HOST_PATH to be provided also when using the cross-built BobUI.
    # This tells the BobUIConfigDependencies file to do appropriate requirement checks.
    if(NOT "${BOBUI_HOST_PATH}" STREQUAL "" AND NOT BOBUI_NO_REQUIRE_HOST_PATH_CHECK)
        set(needed TRUE)
    endif()
    set(${out_var} "${needed}" PARENT_SCOPE)
endfunction()

macro(_bobui_internal_find_host_info_package platform_requires_host_info install_namespace)
    if(${platform_requires_host_info})
        find_package(${install_namespace}HostInfo
                     CONFIG
                     REQUIRED
                     PATHS "${BOBUI_HOST_PATH}"
                           "${BOBUI_HOST_PATH_CMAKE_DIR}"
                     NO_CMAKE_FIND_ROOT_PATH
                     NO_DEFAULT_PATH)
    endif()
endmacro()

macro(_bobui_internal_setup_bobui_host_path
        host_path_required
        initial_bobui_host_path
        initial_bobui_host_path_cmake_dir
    )
    # Set up BOBUI_HOST_PATH and do sanity checks.
    # A host path is required when cross-compiling but optional when doing a native build.
    # Requiredness can be overridden via variable.
    if(DEFINED BOBUI_REQUIRE_HOST_PATH_CHECK)
        set(_bobui_platform_host_path_required "${BOBUI_REQUIRE_HOST_PATH_CHECK}")
    elseif(DEFINED ENV{BOBUI_REQUIRE_HOST_PATH_CHECK})
        set(_bobui_platform_host_path_required "$ENV{BOBUI_REQUIRE_HOST_PATH_CHECK}")
    else()
        set(_bobui_platform_host_path_required "${host_path_required}")
    endif()

    if(_bobui_platform_host_path_required)
        # BOBUI_HOST_PATH precedence:
        # - cache variable / command line option
        # - environment variable
        # - initial BOBUI_HOST_PATH when bobuibase was configured (and the directory exists)
        if(NOT DEFINED BOBUI_HOST_PATH)
            if(DEFINED ENV{BOBUI_HOST_PATH})
                set(BOBUI_HOST_PATH "$ENV{BOBUI_HOST_PATH}" CACHE PATH "")
            elseif(NOT "${initial_bobui_host_path}" STREQUAL "" AND EXISTS "${initial_bobui_host_path}")
                set(BOBUI_HOST_PATH "${initial_bobui_host_path}" CACHE PATH "")
            endif()
        endif()

        if(NOT BOBUI_HOST_PATH STREQUAL "")
            get_filename_component(_bobui_platform_host_path_absolute "${BOBUI_HOST_PATH}" ABSOLUTE)
        endif()

        if("${BOBUI_HOST_PATH}" STREQUAL "" OR NOT EXISTS "${_bobui_platform_host_path_absolute}")
            message(FATAL_ERROR
                "To use a cross-compiled BobUI, please set the BOBUI_HOST_PATH cache variable to the "
                "location of your host BobUI installation.")
        endif()

        # BOBUI_HOST_PATH_CMAKE_DIR is needed to work around the rerooting issue when looking for host
        # tools. See REROOT_PATH_ISSUE_MARKER.
        # Prefer initially configured path if none was explicitly set.
        if(NOT DEFINED BOBUI_HOST_PATH_CMAKE_DIR)
            if(NOT "${initial_bobui_host_path_cmake_dir}" STREQUAL ""
                  AND EXISTS "${initial_bobui_host_path_cmake_dir}")
                set(BOBUI_HOST_PATH_CMAKE_DIR "${initial_bobui_host_path_cmake_dir}" CACHE PATH "")
            else()
                # First try to auto-compute the location instead of requiring to set
                # BOBUI_HOST_PATH_CMAKE_DIR explicitly.
                __bobui_internal_get_possible_cmake_dirs(__bobui_candidate_host_path_cmake_dirs
                    "${BOBUI_HOST_PATH}"
                )
                if(NOT __bobui_candidate_host_path_cmake_dirs STREQUAL "")
                    list(GET __bobui_candidate_host_path_cmake_dirs 0
                        __bobui_candidate_host_path_cmake_dir
                    )
                    set(BOBUI_HOST_PATH_CMAKE_DIR
                        "${__bobui_candidate_host_path_cmake_dir}" CACHE PATH "")
                endif()
            endif()
        endif()

        if(NOT BOBUI_HOST_PATH_CMAKE_DIR STREQUAL "")
            get_filename_component(_bobui_platform_host_path_cmake_dir_absolute
                                   "${BOBUI_HOST_PATH_CMAKE_DIR}" ABSOLUTE)
        endif()

        if("${BOBUI_HOST_PATH_CMAKE_DIR}" STREQUAL ""
                OR NOT EXISTS "${_bobui_platform_host_path_cmake_dir_absolute}")
            message(FATAL_ERROR
                "To use a cross-compiled BobUI, please set the BOBUI_HOST_PATH_CMAKE_DIR cache variable "
                "to the location of your host BobUI installation lib/cmake directory.")
        endif()
    endif()
endmacro()

function(_bobui_internal_show_private_module_warning module)
    if(DEFINED BOBUI_REPO_MODULE_VERSION OR BOBUI_NO_PRIVATE_MODULE_WARNING OR BOBUI_FIND_PRIVATE_MODULES)
        return()
    endif()

    get_cmake_property(warning_shown __bobui_private_module_${module}_warning_shown)
    if(warning_shown)
        return()
    endif()

    message(WARNING
        "This project is using headers of the ${module} module and will therefore be tied "
        "to this specific BobUI module build version. "
        "Running this project against other versions of the BobUI modules may crash at any arbitrary "
        "point. This is not a bug, but a result of using BobUI internals. You have been warned! "
        "\nYou can disable this warning by setting BOBUI_NO_PRIVATE_MODULE_WARNING to ON."
    )
    set_property(GLOBAL PROPERTY __bobui_private_module_${module}_warning_shown TRUE)
endfunction()
