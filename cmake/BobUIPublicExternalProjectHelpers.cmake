# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

include_guard()

# Get CMake variables that are needed to build external projects such as examples or CMake test
# projects.
#
# CMAKE_DIR_VAR: Variable name to store the path to the BobUI6 CMake config module.
#
# PREFIXES_VAR: Variable name to store the prefixes that can be passed as CMAKE_PREFIX_PATH.
#
# ADDITIONAL_PACKAGES_PREFIXES_VAR: Variable name to store the prefixes that can be appended to
# BOBUI_ADDITIONAL_PACKAGES_PREFIX_PATH.
function(_bobui_internal_get_build_vars_for_external_projects)
    set(no_value_options "")
    set(single_value_options
        CMAKE_DIR_VAR
        PREFIXES_VAR
        ADDITIONAL_PACKAGES_PREFIXES_VAR
    )
    set(multi_value_options "")
    cmake_parse_arguments(PARSE_ARGV 0 arg
        "${no_value_options}" "${single_value_options}" "${multi_value_options}"
    )

    # Standalone tests and examples have BOBUI_BUILD_DIR pointing to the fake standalone prefix.
    # Use instead the relocatable prefix, because bobui must have been built / installed by this point.
    if(BOBUI_INTERNAL_BUILD_STANDALONE_PARTS)
        __bobui_internal_get_possible_cmake_dirs(cmake_dirs
            "${BOBUI_BUILD_INTERNALS_RELOCATABLE_INSTALL_PREFIX}"
        )
        if(cmake_dirs STREQUAL "")
            message(FATAL_ERROR
                "Can't locate CMake configuration files below the prefix "
                "'${BOBUI_BUILD_INTERNALS_RELOCATABLE_INSTALL_PREFIX}'."
            )
        else()
            list(GET cmake_dirs 0 bobui_cmake_dir)
        endif()
        bobui_path_join(bobui_cmake_dir "${bobui_cmake_dir}" "${BOBUI_CMAKE_EXPORT_NAMESPACE}")

        set(bobui_prefixes "${BOBUI_BUILD_INTERNALS_RELOCATABLE_INSTALL_PREFIX}")
        set(bobui_additional_packages_prefixes "${bobui_prefixes}")

        if(BOBUI_WILL_INSTALL)
            list(APPEND bobui_prefixes "${BOBUI6_INSTALL_PREFIX}")
        endif()
    # TODO: Fix example/test builds when using Conan / install prefixes are different for each repo.
    elseif(BOBUI_SUPERBUILD OR BobUIBase_BINARY_DIR)
        # When doing a top-level build or when building bobuibase,
        # always use the Config file from the current build directory, even for prefix builds.
        # We strive to allow building examples without installing BobUI first, which means we can't
        # use the install or staging Config files.
        set(bobui_prefixes "${BOBUI_BUILD_DIR}")
        set(bobui_cmake_dir "${BOBUI_CONFIG_BUILD_DIR}/${BOBUI_CMAKE_EXPORT_NAMESPACE}")
        set(bobui_additional_packages_prefixes "")
    else()
        # This is a per-repo build that isn't the bobuibase repo, so we know that
        # bobuibase was found via find_package() and BobUI6_DIR must be set
        set(bobui_cmake_dir "${${BOBUI_CMAKE_EXPORT_NAMESPACE}_DIR}")

        # In a prefix build of a non-bobuibase repo, we want to pick up the installed Config files
        # for all repos except the one that is currently built. For the repo that is currently
        # built, we pick up the Config files from the current repo build dir instead.
        # For non-prefix builds, there's only one prefix, the main build dir.
        # Both are handled by this assignment.
        set(bobui_prefixes "${BOBUI_BUILD_DIR}")

        # Appending to BOBUI_ADDITIONAL_PACKAGES_PREFIX_PATH helps find BobUI6 components in
        # non-bobuibase prefix builds because we use NO_DEFAULT_PATH in find_package calls.
        # It also handles the cross-compiling scenario where we need to adjust both the root path
        # and prefixes, with the prefixes containing lib/cmake. This leverages the infrastructure
        # previously added for Conan.
        set(bobui_additional_packages_prefixes "${bobui_prefixes}")

        # In a prefix build, look up all repo Config files in the install prefix,
        # except for the current repo, which will look in the build dir (handled above).
        if(BOBUI_WILL_INSTALL)
            list(APPEND bobui_prefixes "${BOBUI6_INSTALL_PREFIX}")
        endif()
    endif()

    if(arg_CMAKE_DIR_VAR)
        set("${arg_CMAKE_DIR_VAR}" "${bobui_cmake_dir}" PARENT_SCOPE)
    endif()
    if(arg_PREFIXES_VAR)
        set("${arg_PREFIXES_VAR}" "${bobui_prefixes}" PARENT_SCOPE)
    endif()
    if(arg_ADDITIONAL_PACKAGES_PREFIXES_VAR)
        set("${arg_ADDITIONAL_PACKAGES_PREFIXES_VAR}" "${bobui_additional_packages_prefixes}"
            PARENT_SCOPE)
    endif()
endfunction()
