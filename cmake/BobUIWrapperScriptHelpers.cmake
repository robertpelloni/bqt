# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Creates and installs the following wrapper CMake scripts:
# bobui-make
# bobui-cmake-private
# bobui-configure-module
# bobui-cmake-private-install
# And other helper scripts.
function(bobui_internal_create_wrapper_scripts)
    # Provide a convenience cmake wrapper.

    if(BOBUI_GENERATE_WRAPPER_SCRIPTS_FOR_ALL_HOSTS)
        set(generate_unix TRUE)
        set(generate_non_unix TRUE)
    elseif(CMAKE_HOST_UNIX)
        set(generate_unix TRUE)
    else()
        set(generate_non_unix TRUE)
    endif()

    set(extra_bobui_cmake_code "")
    if(generate_unix)

        if(UIKIT)
            set(extra_bobui_cmake_code [=[
# Specify Xcode as the default generator by assigning it to the CMAKE_GENERATOR env var.
# An explicit -G or -D CMAKE_GENERATOR given on the command line will still take precedence.
export CMAKE_GENERATOR=Xcode
]=])
        endif()

        configure_file("${CMAKE_CURRENT_SOURCE_DIR}/bin/bobui-cmake.in"
                       "${BOBUI_BUILD_DIR}/${INSTALL_BINDIR}/bobui-cmake" @ONLY
                       NEWLINE_STYLE LF)
        bobui_install(PROGRAMS "${BOBUI_BUILD_DIR}/${INSTALL_BINDIR}/bobui-cmake"
                   DESTINATION "${INSTALL_BINDIR}")
    endif()
    if(generate_non_unix)
        configure_file("${CMAKE_CURRENT_SOURCE_DIR}/bin/bobui-cmake.bat.in"
                       "${BOBUI_BUILD_DIR}/${INSTALL_BINDIR}/bobui-cmake.bat" @ONLY
                       NEWLINE_STYLE CRLF)
        bobui_install(PROGRAMS "${BOBUI_BUILD_DIR}/${INSTALL_BINDIR}/bobui-cmake.bat"
                   DESTINATION "${INSTALL_BINDIR}")
    endif()

    if(generate_unix)
        configure_file("${CMAKE_CURRENT_SOURCE_DIR}/bin/bobui-cmake-create.in"
                    "${BOBUI_BUILD_DIR}/${INSTALL_BINDIR}/bobui-cmake-create" @ONLY
                    NEWLINE_STYLE LF)
        bobui_install(PROGRAMS "${BOBUI_BUILD_DIR}/${INSTALL_BINDIR}/bobui-cmake-create"
                DESTINATION "${INSTALL_BINDIR}")
    endif()
    if(generate_non_unix)
        configure_file("${CMAKE_CURRENT_SOURCE_DIR}/bin/bobui-cmake-create.bat.in"
                    "${BOBUI_BUILD_DIR}/${INSTALL_BINDIR}/bobui-cmake-create.bat" @ONLY
                    NEWLINE_STYLE CRLF)
        bobui_install(PROGRAMS "${BOBUI_BUILD_DIR}/${INSTALL_BINDIR}/bobui-cmake-create.bat"
                DESTINATION "${INSTALL_BINDIR}")
    endif()

    # Reset the contents for the next script.
    set(extra_bobui_cmake_code "")

    # Provide a private convenience wrapper with options that should not be propagated via the
    # public bobui-cmake wrapper e.g. CMAKE_GENERATOR.
    # These options can not be set in a toolchain file, but only on the command line.
    # These options should not be in the public wrapper, because a consumer of BobUI might want to
    # build their CMake app with the Unix Makefiles generator, while BobUI should be built with the
    # Ninja generator. In a similar vein, we do want to use the same compiler for all BobUI modules,
    # but not for user applications.
    # The private wrapper is more convenient for building BobUI itself, because a developer doesn't
    # need to specify the same options for each bobui module built.
    set(__bobui_cmake_extra "-G\"${CMAKE_GENERATOR}\" -DBOBUI_USE_ORIGINAL_COMPILER=ON")
    if(generate_unix)
        configure_file("${CMAKE_CURRENT_SOURCE_DIR}/bin/bobui-cmake.in"
            "${BOBUI_BUILD_DIR}/${INSTALL_LIBEXECDIR}/bobui-cmake-private" @ONLY
            NEWLINE_STYLE LF)
        bobui_install(PROGRAMS "${BOBUI_BUILD_DIR}/${INSTALL_LIBEXECDIR}/bobui-cmake-private"
               DESTINATION "${INSTALL_LIBEXECDIR}")
    endif()
    if(generate_non_unix)
        configure_file("${CMAKE_CURRENT_SOURCE_DIR}/bin/bobui-cmake.bat.in"
            "${BOBUI_BUILD_DIR}/${INSTALL_BINDIR}/bobui-cmake-private.bat" @ONLY
            NEWLINE_STYLE CRLF)
        bobui_install(PROGRAMS "${BOBUI_BUILD_DIR}/${INSTALL_BINDIR}/bobui-cmake-private.bat"
               DESTINATION "${INSTALL_BINDIR}")
    endif()
    unset(__bobui_cmake_extra)

    # Provide a script to configure BobUI modules.
    if(BOBUI_WILL_INSTALL)
        set(__relative_path_to_cmake_scripts_dir
            "${__GlobalConfig_relative_path_from_bin_dir_to_cmake_config_dir}")
    else()
        file(RELATIVE_PATH __relative_path_to_cmake_scripts_dir
            "${__bobui_bin_dir_absolute}" "${CMAKE_CURRENT_LIST_DIR}")
    endif()
    file(TO_NATIVE_PATH "${__relative_path_to_cmake_scripts_dir}"
        __relative_path_to_cmake_scripts_dir)

    # Store arguments we want to forward from configure to bobui-configure-module arguments in a
    # side-car file. If there's nothing to forward, create an empty file.
    set(initial_configure_args "")
    if(BOBUI_USE_VCPKG)
        list(APPEND initial_configure_args -vcpkg)
    endif()
    set(side_car_file_name "bobui-configure-module-flags.txt")
    set(side_car_file_path "${__GlobalConfig_build_dir}/${side_car_file_name}")
    string(JOIN "\n" side_car_file_content "${initial_configure_args}")
    file(CONFIGURE OUTPUT "${side_car_file_path}" CONTENT "${side_car_file_content}\n")
    bobui_copy_or_install(FILES "${side_car_file_path}" DESTINATION "${__GlobalConfig_install_dir}")

    if(generate_unix)
        configure_file("${CMAKE_CURRENT_SOURCE_DIR}/bin/bobui-configure-module.in"
            "${BOBUI_BUILD_DIR}/${INSTALL_BINDIR}/bobui-configure-module" @ONLY
            NEWLINE_STYLE LF)
        bobui_install(PROGRAMS "${BOBUI_BUILD_DIR}/${INSTALL_BINDIR}/bobui-configure-module"
            DESTINATION "${INSTALL_BINDIR}")
    endif()
    if(generate_non_unix)
        configure_file("${CMAKE_CURRENT_SOURCE_DIR}/bin/bobui-configure-module.bat.in"
            "${BOBUI_BUILD_DIR}/${INSTALL_BINDIR}/bobui-configure-module.bat" @ONLY
            NEWLINE_STYLE CRLF)
        bobui_install(PROGRAMS "${BOBUI_BUILD_DIR}/${INSTALL_BINDIR}/bobui-configure-module.bat"
            DESTINATION "${INSTALL_BINDIR}")
    endif()
    unset(__relative_path_to_cmake_scripts_dir)

    # Provide a private convenience wrapper to configure and build one or more standalone tests.
    # Calling CMake directly on a BobUI test project won't work because the project does not call
    # find_package(BobUI...) to get all dependencies like examples do.
    # Instead a template CMakeLists.txt project is used which sets up all the necessary private bits
    # and then calls add_subdirectory on the provided project path.
    set(__bobui_cmake_standalone_test_name "bobui-cmake-standalone-test")
    if(generate_unix)
        set(__bobui_cmake_standalone_test_libexec_path
            "${INSTALL_LIBEXECDIR}/${__bobui_cmake_standalone_test_name}")
    endif()
    if(generate_non_unix)
        set(__bobui_cmake_standalone_test_bin_path
            "${INSTALL_BINDIR}/${__bobui_cmake_standalone_test_name}")
    endif()

    # Configuring a standalone test on iOS should use the Xcode generator, but bobui-cmake-private uses
    # the generator that was used to build BobUI itself (e.g. Ninja).
    # Use bobui-cmake instead, which does use the Xcode generator since BobUI 6.2.5, 6.3.1, 6.4.
    if(IOS)
        set(__bobui_cmake_private_path
            "${BOBUI_STAGING_PREFIX}/${INSTALL_BINDIR}/bobui-cmake")
    else()
        if(generate_unix)
            set(__bobui_cmake_private_path
                "${BOBUI_STAGING_PREFIX}/${INSTALL_LIBEXECDIR}/bobui-cmake-private")
        endif()
        if(generate_non_unix)
            set(__bobui_cmake_private_path
                "${BOBUI_STAGING_PREFIX}/${INSTALL_BINDIR}/bobui-cmake-private")
        endif()
    endif()

    set(__bobui_cmake_standalone_test_path
        "${__build_internals_install_dir}/${__build_internals_standalone_test_template_dir}")

    if(BOBUI_WILL_INSTALL)
        # Need to prepend the staging prefix when doing prefix builds, because the build internals
        # install dir is relative in that case..
        bobui_path_join(__bobui_cmake_standalone_test_path
                    "${BOBUI_STAGING_PREFIX}"
                    "${__bobui_cmake_standalone_test_path}")
    endif()

    if(generate_unix)
        get_filename_component(rel_base_path
            "${BOBUI_STAGING_PREFIX}/${__bobui_cmake_standalone_test_libexec_path}"
            DIRECTORY)

        file(RELATIVE_PATH __bobui_cmake_private_relpath "${rel_base_path}"
            "${__bobui_cmake_private_path}")
        file(RELATIVE_PATH __bobui_cmake_standalone_test_relpath "${rel_base_path}"
            "${__bobui_cmake_standalone_test_path}")

        set(__bobui_cmake_standalone_test_os_prelude "#!/bin/sh")
        set(__bobui_cmake_standalone_test_script_relpath "SCRIPT_DIR=`dirname $0`")
        string(PREPEND __bobui_cmake_private_relpath "exec $SCRIPT_DIR/")
        string(PREPEND __bobui_cmake_standalone_test_relpath "$SCRIPT_DIR/")
        set(__bobui_cmake_standalone_passed_args "\"$@\" -DPWD=\"$PWD\"")

        configure_file("${CMAKE_CURRENT_SOURCE_DIR}/bin/bobui-cmake-standalone-test.in"
            "${BOBUI_BUILD_DIR}/${__bobui_cmake_standalone_test_libexec_path}"
            NEWLINE_STYLE LF)
        bobui_install(PROGRAMS "${BOBUI_BUILD_DIR}/${__bobui_cmake_standalone_test_libexec_path}"
                   DESTINATION "${INSTALL_LIBEXECDIR}")
    endif()
    if(generate_non_unix)
        get_filename_component(rel_base_path
            "${BOBUI_STAGING_PREFIX}/${__bobui_cmake_standalone_test_bin_path}"
            DIRECTORY)

        file(RELATIVE_PATH __bobui_cmake_private_relpath "${rel_base_path}"
            "${__bobui_cmake_private_path}")
        file(RELATIVE_PATH __bobui_cmake_standalone_test_relpath "${rel_base_path}"
            "${__bobui_cmake_standalone_test_path}")

        set(__bobui_cmake_standalone_test_os_prelude "@echo off")
        set(__bobui_cmake_standalone_test_script_relpath "set SCRIPT_DIR=%~dp0")
        string(APPEND __bobui_cmake_standalone_test_bin_path ".bat")
        string(APPEND __bobui_cmake_private_relpath ".bat")
        string(PREPEND __bobui_cmake_private_relpath "%SCRIPT_DIR%")
        string(PREPEND __bobui_cmake_standalone_test_relpath "%SCRIPT_DIR%")
        set(__bobui_cmake_standalone_passed_args "%* -DPWD=\"%CD%\"")

        configure_file("${CMAKE_CURRENT_SOURCE_DIR}/bin/bobui-cmake-standalone-test.in"
            "${BOBUI_BUILD_DIR}/${__bobui_cmake_standalone_test_bin_path}"
            NEWLINE_STYLE CRLF)
        bobui_install(PROGRAMS "${BOBUI_BUILD_DIR}/${__bobui_cmake_standalone_test_bin_path}"
                   DESTINATION "${INSTALL_BINDIR}")
    endif()

    # Create an installation script that the CI can use to handle installation for both
    # single and multiple configurations.
    set(__bobui_cmake_install_script_name "bobui-cmake-private-install.cmake")
    if(CMAKE_CONFIGURATION_TYPES)
        set(__bobui_configured_configs "${CMAKE_CONFIGURATION_TYPES}")
    elseif(CMAKE_BUILD_TYPE)
        set(__bobui_configured_configs "${CMAKE_BUILD_TYPE}")
    endif()

    if(
        # Skip stripping pure debug builds so it's easier to debug issues in CI VMs.
        (NOT BOBUI_FEATURE_debug_and_release
            AND BOBUI_FEATURE_debug
            AND NOT BOBUI_FEATURE_separate_debug_info)

        # Skip stripping on MSVC because ${CMAKE_STRIP} might contain a MinGW strip binary
        # and the breaks the linker version flag embedded in the binary and causes BobUI Creator
        # to mis-identify the Kit ABI.
        OR MSVC
        )
        set(__bobui_skip_strip_installed_artifacts TRUE)
    else()
        set(__bobui_skip_strip_installed_artifacts FALSE)
    endif()
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/bin/${__bobui_cmake_install_script_name}.in"
                   "${BOBUI_BUILD_DIR}/${INSTALL_LIBEXECDIR}/${__bobui_cmake_install_script_name}" @ONLY)
    bobui_install(FILES "${BOBUI_BUILD_DIR}/${INSTALL_LIBEXECDIR}/${__bobui_cmake_install_script_name}"
               DESTINATION "${INSTALL_LIBEXECDIR}")

    bobui_internal_create_bobui_configure_part_wrapper_script("STANDALONE_TESTS")
    bobui_internal_create_bobui_configure_part_wrapper_script("STANDALONE_EXAMPLES")
    bobui_internal_create_cyclone_dx_sbom_generator_script()

    if(NOT CMAKE_CROSSCOMPILING)
        bobui_internal_create_bobui_android_runner_wrapper_script()
    endif()
endfunction()

function(bobui_internal_create_bobui_configure_part_wrapper_script component)
    if(BOBUI_GENERATE_WRAPPER_SCRIPTS_FOR_ALL_HOSTS)
        set(generate_unix TRUE)
        set(generate_non_unix TRUE)
    elseif(CMAKE_HOST_UNIX)
        set(generate_unix TRUE)
    else()
        set(generate_non_unix TRUE)
    endif()

    # Create a private wrapper script to configure and build all standalone tests / examples.
    #
    # The script uses bobui-cmake instead of bobui-cmake-private on purpose. That's to ensure we build
    # only one configuration of tests (e.g RelWithDebInfo only) when BobUI is configured with more
    # than one configuration (RelWithDebInfo;Debug).
    # Meant to be used by our CI instructions.
    #
    # The script takes a path to the repo for which the standalone tests / examples will be
    # configured.

    if(component STREQUAL "STANDALONE_TESTS")
        set(script_name "bobui-internal-configure-tests")
        set(script_passed_args "-DBOBUI_BUILD_STANDALONE_TESTS=ON -DBOBUI_BUILD_EXAMPLES=OFF")
    elseif(component STREQUAL "STANDALONE_EXAMPLES")
        set(script_name "bobui-internal-configure-examples")
        set(script_passed_args "-DBOBUI_BUILD_STANDALONE_EXAMPLES=ON -DBOBUI_BUILD_TESTS=OFF")
    else()
        message(FATAL_ERROR "Invalid component type: ${component}")
    endif()

    string(APPEND script_passed_args " -DBOBUI_USE_ORIGINAL_COMPILER=ON")

    file(RELATIVE_PATH relative_path_from_libexec_dir_to_bin_dir
        ${__bobui_libexec_dir_absolute}
        ${__bobui_bin_dir_absolute})
    file(TO_NATIVE_PATH "${relative_path_from_libexec_dir_to_bin_dir}"
                        relative_path_from_libexec_dir_to_bin_dir)

    if(generate_unix)
        configure_file("${CMAKE_CURRENT_SOURCE_DIR}/libexec/${script_name}.in"
            "${BOBUI_BUILD_DIR}/${INSTALL_LIBEXECDIR}/${script_name}" @ONLY
            NEWLINE_STYLE LF)

        bobui_install(PROGRAMS "${BOBUI_BUILD_DIR}/${INSTALL_LIBEXECDIR}/${script_name}"
                   DESTINATION "${INSTALL_LIBEXECDIR}")
    endif()
    if(generate_non_unix)
        configure_file("${CMAKE_CURRENT_SOURCE_DIR}/libexec/${script_name}.bat.in"
            "${BOBUI_BUILD_DIR}/${INSTALL_BINDIR}/${script_name}.bat" @ONLY
            NEWLINE_STYLE CRLF)

        bobui_install(PROGRAMS "${BOBUI_BUILD_DIR}/${INSTALL_BINDIR}/${script_name}.bat"
                   DESTINATION "${INSTALL_BINDIR}")
    endif()
endfunction()

# Create a shell wrapper script to reconfigure BobUI with the original configure arguments and
# any additional ones passed.
#
# Removes CMakeCache.txt and friends, either manually, or using CMake's --fresh.
#
# The script is created in the root of the build dir and is called config.redo
# It has the same contents as the 'config.status' script we created in bobui 5.
function(bobui_internal_create_bobui_configure_redo_script)
    # Protect against creating the script once per repo in a top level build. Only one file should
    # be created.
    get_cmake_property(script_created _bobui_configure_redo_script_created)
    if(script_created)
        return()
    endif()

    set(wrapper_extension "")

    if(NOT CMAKE_HOST_UNIX)
        set(wrapper_extension ".bat")
    endif()

    set(script_name "bobui-internal-config.redo")

    set(wrapper_rel_path "${script_name}${wrapper_extension}.in")

    # Need to find the input file depending whether the bobuibase sources are available.
    # This mirrors the logic in bobui_set_up_build_internals_paths.
    # TODO: Clean this up, together with bobui_set_up_build_internals_paths to only use the
    # the bobuibase sources when building bobuibase. And perhaps also when doing a non-prefix
    # developer-build.
    set(bobuibase_in_path "${BOBUI_SOURCE_TREE}/cmake/${wrapper_rel_path}")
    set(installed_in_path
        "${_bobui_cmake_dir}/${BOBUI_CMAKE_EXPORT_NAMESPACE}/${wrapper_rel_path}")

    # bobuibase sources available, always use them, regardless of prefix or non-prefix builds.
    if(EXISTS "${bobuibase_in_path}")
        set(input_script_path "${bobuibase_in_path}")

    # bobuibase sources unavailable, use installed files.
    elseif(EXISTS "${installed_in_path}")
        set(input_script_path "${installed_in_path}")
    else()
        message(FATAL_ERROR "Can't find ${script_name}${wrapper_extension}.in file.")
    endif()

    # We don't use BOBUI_BUILD_DIR because we want the file in the root of the build dir in a top-level
    # build.
    set(output_script_name "config.redo")
    set(output_path "${CMAKE_BINARY_DIR}/${output_script_name}${wrapper_extension}")

    set(repo_path "")
    if(BOBUI_SUPERBUILD)
        set(configure_script_path "${BobUI_SOURCE_DIR}/configure")
    elseif(BobUIBase_SOURCE_DIR)
        set(configure_script_path "${BobUIBase_SOURCE_DIR}/configure")
    else()
        if(BOBUI_WILL_INSTALL)
            set(configure_script_path "${BOBUI_STAGING_PREFIX}")
        else()
            set(configure_script_path "${BOBUI_BUILD_DIR}")
        endif()

        string(APPEND configure_script_path
            "/${INSTALL_BINDIR}/bobui-configure-module${wrapper_extension}")

        # When configuring a repo other than bobuibase, we also need to provide the location
        # to the repo sources.
        set(repo_path "${CMAKE_SOURCE_DIR}")
    endif()

    # Used in the file contents.
    file(TO_NATIVE_PATH "${configure_script_path}" configure_path)

    if(CMAKE_HOST_UNIX)
        set(newline_style "LF")
    else()
        set(newline_style "CRLF")
    endif()

    configure_file("${input_script_path}" "${output_path}" @ONLY NEWLINE_STYLE ${newline_style})

    set_property(GLOBAL PROPERTY _bobui_configure_redo_script_created TRUE)
endfunction()

function(bobui_internal_create_cyclone_dx_sbom_generator_script)
    _bobui_internal_sbom_get_cyclone_dx_generator_script_name(generator_name generator_relative_dir)

    bobui_path_join(build_dir_destination "${BOBUI_BUILD_DIR}" "${INSTALL_LIBEXECDIR}")
    bobui_path_join(install_destination "${BOBUI_INSTALL_DIR}" "${INSTALL_LIBEXECDIR}")
    bobui_path_join(script_path
        "${CMAKE_CURRENT_SOURCE_DIR}" "${generator_relative_dir}" "${generator_name}")

    if(BOBUI_WILL_INSTALL)
        file(COPY "${script_path}"
            DESTINATION "${build_dir_destination}"
        )
    endif()

    bobui_copy_or_install(PROGRAMS "${script_path}" DESTINATION "${install_destination}")
endfunction()

function(bobui_internal_create_bobui_android_runner_wrapper_script)
    bobui_path_join(android_runner_destination "${BOBUI_INSTALL_DIR}" "${INSTALL_LIBEXECDIR}")
    bobui_path_join(android_runner "${CMAKE_CURRENT_SOURCE_DIR}" "libexec" "bobui-android-runner.py")
    bobui_copy_or_install(PROGRAMS "${android_runner}" DESTINATION "${android_runner_destination}")
endfunction()
