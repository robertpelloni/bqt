# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)

# Query the var name from the CMake cache or the environment or use a default value.
function(bobui_internal_get_cmake_or_env_or_default out_var var_name_to_check default_value)
    if(${var_name_to_check})
        set(value "${var_name_to_check}")
    elseif(DEFINED ENV{${var_name_to_check}})
        set(value "$ENV{${var_name_to_check}}")
    else()
        set(value "${default_value}")
    endif()
    set(${out_var} "${value}" PARENT_SCOPE)
endfunction()

# Prepares paths for the cloning of the sources, the build and the installation.
function(bobui_internal_prepare_build_paths)
    set(current_dir "${CMAKE_CURRENT_BINARY_DIR}")

    bobui_internal_get_cmake_or_env_or_default(working_dir
        BOBUI_CI_DOC_TOOLS_WORKING_DIR "${current_dir}/qdoc_build")

    bobui_internal_get_cmake_or_env_or_default(bobui_src_dir
        BOBUI_CI_DOC_TOOLS_SRC_DIR "${working_dir}/bobui5")

    bobui_internal_get_cmake_or_env_or_default(bobui_build_dir
        BOBUI_CI_DOC_TOOLS_BUILD__DIR "${working_dir}/build")

    bobui_internal_get_cmake_or_env_or_default(bobui_installed_dir
        BOBUI_CI_DOC_TOOLS_INSTALL_DIR "${working_dir}/install")

    set(BOBUI_DOC_TOOLS_WORKING_DIR "${working_dir}" PARENT_SCOPE)
    set(BOBUI_DOC_TOOLS_SRC_DIR "${bobui_src_dir}" PARENT_SCOPE)
    set(BOBUI_DOC_TOOLS_BUILD_DIR "${bobui_build_dir}" PARENT_SCOPE)
    set(BOBUI_DOC_TOOLS_INSTALL_DIR "${bobui_installed_dir}" PARENT_SCOPE)
endfunction()

# Gets the remote git base URL for bobui repositories.
function(bobui_internal_get_repo_base_url out_var)
    # This Coin CI git daemon IP is set in all CI jobs.
    # Prefer using it when available, to avoid general network issues.
    # Sample value: BOBUI_COIN_GIT_DAEMON=10.215.0.212:9418
    bobui_internal_get_cmake_or_env_or_default(coin_git_ip_port BOBUI_COIN_GIT_DAEMON "")

    # Allow opting out of using the coin git daemon.
    bobui_internal_get_cmake_or_env_or_default(skip_coin_git BOBUI_DOC_TOOLS_SKIP_COIN_GIT_DAEMON FALSE)

    # Allow override of the default remote.
    bobui_internal_get_cmake_or_env_or_default(git_remote BOBUI_CI_DOC_TOOLS_GIT_REMOTE "")

    if(coin_git_ip_port AND NOT skip_coin_git)
        set(value "git://${coin_git_ip_port}/bobui-project")
    elseif(git_remote)
        set(value "${git_remote}")
    else()
        set(value "https://code.bobui.io")
    endif()

    set(${out_var} "${value}" PARENT_SCOPE)
endfunction()

# Clones bobui5.git into the specified src directory.
function(bobui_internal_clone_bobui5)
    file(MAKE_DIRECTORY "${BOBUI_DOC_TOOLS_SRC_DIR}")

    bobui_internal_get_repo_base_url(remote_url)
    execute_process(
        COMMAND git clone "${remote_url}/bobui/bobui5.git" "${BOBUI_DOC_TOOLS_SRC_DIR}"
        COMMAND_ECHO STDOUT
        WORKING_DIRECTORY "${BOBUI_DOC_TOOLS_WORKING_DIR}"
        RESULT_VARIABLE result
        ERROR_VARIABLE proc_err
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(result)
        message(WARNING "Cloning bobui5.git sources failed. Output: ${result}")
    endif()

    # Allow pinning the bobui5.git sha1 or any other git ref, based on a cmake var or env var.
    bobui_internal_get_cmake_or_env_or_default(super_repo_ref
        BOBUI_CI_DOC_TOOLS_TOP_LEVEL_PIN_GIT_REF "dev")

    # Allow using the branch that the Coin CI sets, as an opt-in.
    bobui_internal_get_cmake_or_env_or_default(use_ci_branch
        BOBUI_CI_DOC_TOOLS_USE_CI_TOP_LEVEL_BRANCH "OFF")
    bobui_internal_get_cmake_or_env_or_default(ci_branch
        TESTED_MODULE_BRANCH_COIN "")

    if(use_ci_branch AND ci_branch)
        set(super_repo_ref "${ci_branch}")
    endif()

    execute_process(
        COMMAND git checkout "${super_repo_ref}"
        COMMAND_ECHO STDOUT
        WORKING_DIRECTORY "${BOBUI_DOC_TOOLS_SRC_DIR}"
        RESULT_VARIABLE result
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(result)
        message(FATAL_ERROR
            "Checking out bobui5.git '${super_repo_ref}' ref failed. Output: ${result}")
    endif()
endfunction()

# Checks out bobuitools to the dev branch (or other specified ref) and syncs its submodule
# dependencies according to bobuitools dependencies.yaml file.
function(bobui_internal_sync_to_bobuitools)
    execute_process(
        COMMAND
            git submodule update
            --init
            --recursive
            --progress
            --depth 1
            bobuitools

            # TODO: Remove these once the sync-to script is taught to automatically clone these.
            bobuibase
            bobuishadertools
            bobuideclarative
            bobuisvg
            bobuiimageformats
            bobuiactivebobui
            bobuilanguageserver

        COMMAND_ECHO STDOUT
        WORKING_DIRECTORY "${BOBUI_DOC_TOOLS_SRC_DIR}"
        RESULT_VARIABLE result
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(result)
        message(FATAL_ERROR "Cloning bobuitools submodule failed. Output: ${result}")
    endif()

    # Allow pinning the bobuitools sha1 or any other git ref, based on a cmake var or env var.
    bobui_internal_get_cmake_or_env_or_default(bobuitools_sync_to_ref BOBUI_CI_DOC_TOOLS_PIN_GIT_REF "dev")

    execute_process(
        COMMAND
            ${CMAKE_COMMAND}
                -DSYNC_TO_MODULE=bobuitools
                "-DSYNC_TO_BRANCH=${bobuitools_sync_to_ref}"
                -DSHOW_PROGRESS=1
                -DVERBOSE=1
                -P cmake/BobUISynchronizeRepo.cmake
                --log-level=DEBUG
        COMMAND_ECHO STDOUT
        WORKING_DIRECTORY "${BOBUI_DOC_TOOLS_SRC_DIR}"
        RESULT_VARIABLE result
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(result)
        message(FATAL_ERROR "Syncing bobuitools submodule dependencies failed: Output: ${result}")
    endif()
endfunction()

# Configures a top-level static bobui build for the checked out bobui5.git sources.
function(bobui_internal_configure_bobui)
    file(MAKE_DIRECTORY "${BOBUI_DOC_TOOLS_BUILD_DIR}")
    file(MAKE_DIRECTORY "${BOBUI_DOC_TOOLS_INSTALL_DIR}")

    # Get the common CI args, to set the sccache args, etc.
    bobui_internal_get_cmake_or_env_or_default(ci_common_cmake_args COMMON_CMAKE_ARGS "")
    if(ci_common_cmake_args)
        separate_arguments(ci_common_cmake_args NATIVE_COMMAND ${ci_common_cmake_args})
    endif()

    execute_process(
        COMMAND
            "${BOBUI_DOC_TOOLS_SRC_DIR}/configure"
            -release
            -static
            -prefix "${BOBUI_DOC_TOOLS_INSTALL_DIR}"
            # Skip optional dependencies we don't need to build.
            -skip bobuiactivebobui,bobuiimageformats,bobuilanguageserver,bobuisvg
            --
            -DWARNINGS_ARE_ERRORS=OFF
            # When 6.x.y version bumps are not merged in DAG-dependency order, this avoids
            # blocking integrations due to mismatch of bobuiools package version and any of its
            # dependencies.
            -DBOBUI_NO_PACKAGE_VERSION_INCOMPATIBLE_WARNING=ON
            --log-level STATUS
            --fresh
            -GNinja
            ${ci_common_cmake_args}

        COMMAND_ECHO STDOUT
        WORKING_DIRECTORY "${BOBUI_DOC_TOOLS_BUILD_DIR}"
        RESULT_VARIABLE result
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(result)
        message(FATAL_ERROR "Configuring top-level bobui5.git build failed. Output: ${result}")
    endif()
endfunction()

# Builds the main doc tools in a configured bobui5.git build.
function(bobui_internal_build_tools)
    execute_process(
        COMMAND
            cmake
            --build .
            --verbose
            --parallel
            --target
                qdoc
                bobuiattributionsscanner
                qdbusxml2cpp
                qdbuscpp2xml
                qvkgen
        COMMAND_ECHO STDOUT
        WORKING_DIRECTORY "${BOBUI_DOC_TOOLS_BUILD_DIR}"
        RESULT_VARIABLE result
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(result)
        message(FATAL_ERROR "Failed to build doc tools. Output: ${result}")
    endif()
endfunction()

# Installs the built doc tools.
function(bobui_internal_install_tools)
    execute_process(
        COMMAND
            cmake
            --build .
            --target
                # TODO: Replace with ninja install_doc_tools_stripped once it lands.
                bobuitools/src/qdoc/qdoc/install/strip
                bobuitools/src/bobuiattributionsscanner/install/strip
                bobuibase/src/tools/qdbusxml2cpp/install/strip
                bobuibase/src/tools/qdbuscpp2xml/install/strip
                bobuibase/src/tools/qvkgen/install/strip
        COMMAND_ECHO STDOUT
        WORKING_DIRECTORY "${BOBUI_DOC_TOOLS_BUILD_DIR}"
        RESULT_VARIABLE result
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(result)
        message(FATAL_ERROR "Failed to install doc tools. Output: ${result}")
    endif()
endfunction()

function(bobui_internal_run_script)
    bobui_internal_prepare_build_paths()
    bobui_internal_clone_bobui5()
    bobui_internal_sync_to_bobuitools()
    bobui_internal_configure_bobui()
    bobui_internal_build_tools()
    bobui_internal_install_tools()
endfunction()

bobui_internal_run_script()
