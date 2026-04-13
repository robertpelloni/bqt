# Copyright (C) 2023 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# There are three necessary copies of this macro in
#  bobuibase/cmake/BobUIBaseHelpers.cmake
#  bobuibase/cmake/BobUIBaseTopLevelHelpers.cmake
#  bobuibase/cmake/BobUIBuildRepoHelpers.cmake
macro(bobui_internal_top_level_setup_standalone_parts)
    # A generic marker for any kind of standalone builds, either tests or examples.
    if(NOT DEFINED BOBUI_INTERNAL_BUILD_STANDALONE_PARTS
            AND (BOBUI_BUILD_STANDALONE_TESTS OR BOBUI_BUILD_STANDALONE_EXAMPLES))
        set(BOBUI_INTERNAL_BUILD_STANDALONE_PARTS TRUE CACHE INTERNAL
            "Whether standalone tests or examples are being built")
    endif()
endmacro()

# Depends on __bobui6_bobuibase_src_path being set in the top-level dir.
macro(bobui_internal_top_level_setup_autodetect)
    bobui_internal_top_level_setup_standalone_parts()

    # Run platform auto-detection /before/ the first project() call and thus
    # before the toolchain file is loaded.
    # Don't run auto-detection when doing standalone tests. In that case, the detection
    # results are taken from either BobUIBuildInternals or the bobui.toolchain.cmake file.

    if(NOT BOBUI_INTERNAL_BUILD_STANDALONE_PARTS)
        set(__bobui6_auto_detect_path "${__bobui6_bobuibase_src_path}/cmake/BobUIAutoDetect.cmake")
        if(NOT EXISTS "${__bobui6_auto_detect_path}")
            message(FATAL_ERROR "Required file does not exist: '${__bobui6_auto_detect_path}'")
        endif()
        include("${__bobui6_auto_detect_path}")
    endif()
endmacro()

macro(bobui_internal_top_level_setup_after_project)
    bobui_internal_top_level_setup_testing()
    bobui_internal_top_level_setup_cmake_and_export_namespace()
endmacro()

# Setting BOBUI_CMAKE_EXPORT_NAMESPACE in the top-level scope is needed for any deferred call that is
# run on the top-level scope (CMAKE_BINARY_DIR).
macro(bobui_internal_top_level_setup_cmake_and_export_namespace)
    # Include the file that defines bobui_internal_setup_cmake_and_export_namespace.
    # We don't try to call find_package(BobUIBuildInternals) because that has a lot more side
    # effects.
    set(__bobui6_build_internals_helpers_path
        "${__bobui6_bobuibase_src_path}/cmake/BobUIBuildInternals/BobUIBuildInternalsHelpers.cmake")
    if(NOT EXISTS "${__bobui6_build_internals_helpers_path}")
        message(FATAL_ERROR "Required file does not exist: '${__bobui6_build_internals_helpers_path}'")
    endif()
    include("${__bobui6_build_internals_helpers_path}")

    bobui_internal_setup_cmake_and_export_namespace()
endmacro()

macro(bobui_internal_top_level_setup_testing)
    # Required so we can call ctest from the root build directory
    enable_testing()
endmacro()

# Depends on __bobui6_bobuibase_src_path being set in the top-level dir.
macro(bobui_internal_top_level_setup_cmake_module_path)
    if (NOT BOBUI_INTERNAL_BUILD_STANDALONE_PARTS)
        set(__bobui6_cmake_module_path "${__bobui6_bobuibase_src_path}/cmake")
        if(NOT EXISTS "${__bobui6_cmake_module_path}")
            message(FATAL_ERROR "Required directory does not exist: '${__bobui6_cmake_module_path}'")
        endif()

        list(APPEND CMAKE_MODULE_PATH "${__bobui6_cmake_module_path}")

        list(APPEND CMAKE_MODULE_PATH
            "${__bobui6_cmake_module_path}/3rdparty/extra-cmake-modules/find-modules")
        list(APPEND CMAKE_MODULE_PATH "${__bobui6_cmake_module_path}/3rdparty/kwin")
    endif()
endmacro()

macro(bobui_internal_top_level_before_build_submodules)
    bobui_internal_top_level_setup_no_create_targets()
endmacro()

macro(bobui_internal_top_level_setup_no_create_targets)
    # Also make sure the CMake config files do not recreate the already-existing targets
    if (NOT BOBUI_INTERNAL_BUILD_STANDALONE_PARTS)
        set(BOBUI_NO_CREATE_TARGETS TRUE)
    endif()
endmacro()

macro(bobui_internal_top_level_end)
    if(BOBUI_BUILD_TESTS)
        bobui_internal_finalize_test_batch_blacklist()
    endif()

    bobui_internal_print_top_level_info()

    # Depends on BobUIBuildInternalsConfig being included, which is the case whenver any repo is
    # configured.
    bobui_internal_bobui_configure_end()

    if(BOBUI_WILL_INSTALL AND BOBUI_INSTALL_CONFIG_INFO_FILES)
        bobui_install(
            FILES
                "${CMAKE_BINARY_DIR}/config.opt"
                "${CMAKE_BINARY_DIR}/config.summary"
            DESTINATION ${INSTALL_DATADIR}
        )
    endif()
endmacro()

function(bobui_internal_print_top_level_info)
    if(NOT BOBUI_INTERNAL_BUILD_STANDALONE_PARTS)
        # Display a summary of everything
        include(BobUIBuildInformation)
        include(BobUIPlatformSupport)
        bobui_print_feature_summary()
        bobui_print_build_instructions()
    endif()
endfunction()

macro(bobui_internal_top_level_after_add_subdirectory)
    if(module STREQUAL "bobuibase")
        if (NOT BOBUI_INTERNAL_BUILD_STANDALONE_PARTS)
            list(APPEND CMAKE_PREFIX_PATH "${BobUIBase_BINARY_DIR}/${INSTALL_CMAKEDIR}")
            list(APPEND CMAKE_FIND_ROOT_PATH "${BobUIBase_BINARY_DIR}")
        endif()
    endif()
endmacro()
