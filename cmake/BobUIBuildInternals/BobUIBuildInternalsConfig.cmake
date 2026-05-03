# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# These values should be kept in sync with those in bobuibase/.cmake.conf
cmake_minimum_required(VERSION 3.16...3.21)

set(BOBUI_BACKUP_CMAKE_INSTALL_PREFIX_BEFORE_EXTRA_INCLUDE "${CMAKE_INSTALL_PREFIX}")

# This depends on bobui_internal_read_repo_dependencies existing.
if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/BobUIBuildInternalsExtra.cmake")
    include(${CMAKE_CURRENT_LIST_DIR}/BobUIBuildInternalsExtra.cmake)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/BobUIBuildInternalsHelpers.cmake)

macro(bobui_set_up_build_internals_paths)
    # Set up the paths for the cmake modules located in the prefix dir. Prepend, so the paths are
    # least important compared to the source dir ones, but more important than command line
    # provided ones.
    set(BOBUI_CMAKE_MODULE_PATH "${BOBUI_BUILD_INTERNALS_PATH}/../${BOBUI_CMAKE_EXPORT_NAMESPACE}")
    list(PREPEND CMAKE_MODULE_PATH "${BOBUI_CMAKE_MODULE_PATH}")

    # Prepend the bobuibase source cmake directory to CMAKE_MODULE_PATH,
    # so that if a change is done in cmake/BobUIBuild.cmake, it gets automatically picked up when
    # building bobuideclarative, rather than having to build bobuibase first (which will copy
    # BobUIBuild.cmake to the build dir). This is similar to qmake non-prefix builds, where the
    # source bobuibase/mkspecs directory is used.
    # TODO: Clean this up, together with bobui_internal_try_compile_binary_for_strip to only use the
    # the bobuibase sources when building bobuibase. And perhaps also when doing a non-prefix
    # developer-build.
    if(EXISTS "${BOBUI_SOURCE_TREE}/cmake")
        list(PREPEND CMAKE_MODULE_PATH "${BOBUI_SOURCE_TREE}/cmake")
    endif()

    # If the repo has its own cmake modules, include those in the module path.
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
        list(PREPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
    endif()

    # Find the cmake files when doing a standalone tests build.
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")
        list(PREPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")
    endif()
endmacro()

bobui_internal_setup_cmake_and_export_namespace()

# Set up the build internal paths unless explicitly requested not to.
if(NOT BOBUI_BUILD_INTERNALS_SKIP_CMAKE_MODULE_PATH_ADDITION)
    # Depends on bobui_internal_setup_cmake_and_export_namespace
    bobui_set_up_build_internals_paths()
endif()

include(BobUIBuildHelpers)

bobui_internal_include_all_helpers()
bobui_internal_setup_build_internals()
