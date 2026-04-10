# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause
#
# Note that this file is not installed.

# Bail out if any part of the build directory's path is symlinked.
function(bobui_internal_check_if_path_has_symlinks path)
    get_filename_component(dir "${path}" ABSOLUTE)
    set(is_symlink FALSE)
    if(CMAKE_HOST_WIN32)
        # CMake marks Windows mount points as symbolic links, so use simplified REALPATH check
        # on Windows platforms instead of IS_SYMLINK.
        get_filename_component(dir_realpath "${dir}" REALPATH)
        if(NOT dir STREQUAL dir_realpath)
            set(is_symlink TRUE)
        endif()
    else()
        while(TRUE)
            if(IS_SYMLINK "${dir}")
                set(is_symlink TRUE)
                break()
            endif()

            set(prev_dir "${dir}")
            get_filename_component(dir "${dir}" DIRECTORY)
            if("${dir}" STREQUAL "${prev_dir}")
                return()
            endif()
        endwhile()
    endif()
    if(is_symlink)
        set(possible_solutions_for_resolving_symlink [[
    - Map directories using a transparent mechanism such as mount --bind
    - Pass the real path of the build directory to CMake, e.g. using
      cd $(realpath <path>) before invoking cmake <source_dir>.
            ]])
        if(BOBUI_ALLOW_SYMLINK_IN_PATHS)
            # In some cases, e.g., Homebrew, it is beneficial to skip this check.
            # Before this, Homebrew had to patch this out to be able to get their build.
            message(WARNING
                "The path \"${path}\" contains symlinks. "
                "This is not recommended, and it may lead to unexpected issues. If you do "
                "not have a good reason for enabling 'BOBUI_ALLOW_SYMLINK_IN_PATHS', disable "
                "it, and follow one of the following solutions: \n"
                "${possible_solutions_for_resolving_symlink} ")
        else()
            message(FATAL_ERROR
                "The path \"${path}\" contains symlinks. "
                "This is not supported. Possible solutions: \n"
                "${possible_solutions_for_resolving_symlink} ")
        endif()
    endif()
endfunction()

# There are three necessary copies of this macro in
#  bobuibase/cmake/BobUIBaseHelpers.cmake
#  bobuibase/cmake/BobUIBaseTopLevelHelpers.cmake
#  bobuibase/cmake/BobUIBuildRepoHelpers.cmake
macro(bobui_internal_bobuibase_setup_standalone_parts)
    # A generic marker for any kind of standalone builds, either tests or examples.
    if(NOT DEFINED BOBUI_INTERNAL_BUILD_STANDALONE_PARTS
            AND (BOBUI_BUILD_STANDALONE_TESTS OR BOBUI_BUILD_STANDALONE_EXAMPLES))
        set(BOBUI_INTERNAL_BUILD_STANDALONE_PARTS TRUE CACHE INTERNAL
            "Whether standalone tests or examples are being built")
    endif()
endmacro()

macro(bobui_internal_bobuibase_run_autodetect)
    bobui_internal_bobuibase_setup_standalone_parts()

    # Run auto detection routines, but not when doing standalone tests or standalone examples.
    # In that case, the detection
    # results are taken from either BobUIBuildInternals or the bobui.toolchain.cmake file. Also, inhibit
    # auto-detection in a top-level build, because the top-level project file already includes it.
    if(NOT BOBUI_INTERNAL_BUILD_STANDALONE_PARTS AND NOT BOBUI_SUPERBUILD)
        include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/BobUIAutoDetect.cmake)
    endif()
endmacro()

macro(bobui_internal_bobuibase_pre_project_setup)
    if(NOT BOBUI_INTERNAL_BUILD_STANDALONE_PARTS)
        # Should this BobUI be static or dynamically linked?
        option(BUILD_SHARED_LIBS "Build BobUI statically or dynamically" ON)
        set(BOBUI_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})

        # This variable is also set in BobUI6CoreConfigExtras.cmake, but it's not loaded when building
        # bobuibase. Set it here so bobui_add_plugin can compute the proper plugin flavor.
        set(BOBUI6_IS_SHARED_LIBS_BUILD ${BUILD_SHARED_LIBS})

        # BUILD_SHARED_LIBS influences the minimum required CMake version. The value is set either
        # by:
        #   a cache variable provided on the configure command line
        #   or set by BobUIAutoDetect.cmake depending on the platform
        #   or specified via a toolchain file that is loaded by the project() call
        #   or set by the option() call above
        include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/BobUICMakeVersionHelpers.cmake")
        include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/BobUIPublicCMakeVersionHelpers.cmake")
        bobui_internal_check_and_warn_about_unsuitable_cmake_version()

        include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/BobUIPublicCMakeEarlyPolicyHelpers.cmake")

        ## Add some paths to check for cmake modules:
        list(PREPEND CMAKE_MODULE_PATH
            "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
            "${CMAKE_CURRENT_SOURCE_DIR}/cmake/3rdparty/extra-cmake-modules/find-modules"
            "${CMAKE_CURRENT_SOURCE_DIR}/cmake/3rdparty/kwin"
        )

        if(MACOS)
            # Add module directory to pick up custom Info.plist template
            list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/macos")
        elseif(IOS)
            # Add module directory to pick up custom Info.plist template
            list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/ios")
        endif()

        ## Find the build internals package.
        set(BOBUI_BUILD_INTERNALS_SKIP_CMAKE_MODULE_PATH_ADDITION TRUE)
        list(PREPEND CMAKE_PREFIX_PATH
            "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
        )
        find_package(BobUIBuildInternals CMAKE_FIND_ROOT_PATH_BOTH)
        unset(BOBUI_BUILD_INTERNALS_SKIP_CMAKE_MODULE_PATH_ADDITION)
    else()
        # When building standalone parts, an istalled BuildInternals package already exists.
        find_package(BobUI6 REQUIRED COMPONENTS BuildInternals CMAKE_FIND_ROOT_PATH_BOTH)
    endif()
endmacro()

macro(bobui_internal_bobuibase_install_mkspecs)
    # As long as we use the mkspecs (for qplatformdefs.h), we need to always
    # install it, especially when cross-compiling.
    set(mkspecs_install_dir "${INSTALL_MKSPECSDIR}")
    bobui_path_join(mkspecs_install_dir ${BOBUI_INSTALL_DIR} ${mkspecs_install_dir})

    file(GLOB mkspecs_subdirs
        LIST_DIRECTORIES TRUE
        "${PROJECT_SOURCE_DIR}/mkspecs/*")
    foreach(entry IN LISTS mkspecs_subdirs)
        if(IS_DIRECTORY ${entry})
            bobui_copy_or_install(DIRECTORY "${entry}"
                               DESTINATION ${mkspecs_install_dir}
                               USE_SOURCE_PERMISSIONS)
        else()
            bobui_copy_or_install(FILES "${entry}"
                               DESTINATION ${mkspecs_install_dir})
        endif()

        # In prefix builds we also need to copy the files into the build dir.
        if(BOBUI_WILL_INSTALL)
            file(COPY "${entry}" DESTINATION "${mkspecs_install_dir}")
        endif()
    endforeach()
endmacro()

function(bobui_internal_bobuibase_install_wayland_files)
    bobui_path_join(wlprotocols_build_dir
                 ${BOBUI_BUILD_DIR}
                 ${INSTALL_BOBUI_SHAREDIR}/wayland/protocols)
    bobui_path_join(wlprotocols_install_dir
                 ${BOBUI_INSTALL_DIR}
                 ${INSTALL_BOBUI_SHAREDIR}/wayland/protocols)

    file(GLOB wlprotocols_subdirs
        LIST_DIRECTORIES TRUE
        "${PROJECT_SOURCE_DIR}/src/3rdparty/wayland/protocols/*")
    foreach(entry IN LISTS wlprotocols_subdirs)
        if (IS_DIRECTORY "${entry}")
            bobui_copy_or_install(DIRECTORY "${entry}"
                               DESTINATION "${wlprotocols_install_dir}"
                               USE_SOURCE_PERMISSIONS)
            if(BOBUI_WILL_INSTALL)
                file(COPY "${entry}" DESTINATION "${wlprotocols_build_dir}")
            endif()
        else()
            bobui_copy_or_install(FILES "${entry}"
                               DESTINATION "${wlprotocols_install_dir}")
            if(BOBUI_WILL_INSTALL)
                file(COPY "${entry}" DESTINATION "${wlprotocols_build_dir}")
            endif()
        endif()
    endforeach()

    bobui_path_join(wlextensions_build_dir
                 ${BOBUI_BUILD_DIR}
                 ${INSTALL_BOBUI_SHAREDIR}/wayland/extensions)
    bobui_path_join(wlextensions_install_dir
                 ${BOBUI_INSTALL_DIR}
                 ${INSTALL_BOBUI_SHAREDIR}/wayland/extensions)

    file(GLOB wlextensions_subdirs
        LIST_DIRECTORIES TRUE
        "${PROJECT_SOURCE_DIR}/src/3rdparty/wayland/extensions/*")
    foreach(entry IN LISTS wlextensions_subdirs)
        if (IS_DIRECTORY "${entry}")
            bobui_copy_or_install(DIRECTORY "${entry}"
                               DESTINATION "${wlextensions_install_dir}"
                               USE_SOURCE_PERMISSIONS)
            if(BOBUI_WILL_INSTALL)
                file(COPY "${entry}" DESTINATION "${wlextensions_build_dir}")
            endif()
        else()
            bobui_copy_or_install(FILES "${entry}"
                               DESTINATION "${wlextensions_install_dir}")
            if(BOBUI_WILL_INSTALL)
                file(COPY "${entry}" DESTINATION "${wlextensions_build_dir}")
            endif()
        endif()
    endforeach()
endfunction()

macro(bobui_internal_bobuibase_build_repo)
    bobui_internal_bobuibase_pre_project_setup()

    bobui_internal_project_setup()

    bobui_build_repo_begin()

    if(NOT BOBUI_INTERNAL_BUILD_STANDALONE_PARTS)
        ## Should this BobUI be built with Werror?
        option(WARNINGS_ARE_ERRORS "Build BobUI with warnings as errors" ${FEATURE_developer_build})

        ## Should this BobUI create versioned hard link for some tools?
        option(BOBUI_CREATE_VERSIONED_HARD_LINK "Enable the use of versioned hard link" ON)

        ## BobUIBase specific configure tests:
        include(BobUIBaseConfigureTests)

        ## Build System tests:
        include(BobUIBaseCMakeTesting)

        ## Include CoreMacros() for bobui6_generate_meta_types()
        ## Also needed for the BobUIBaseGlobalTargets creation.
        set(BOBUI_DEFAULT_MAJOR_VERSION 6)
        include(src/corelib/BobUI6CoreMacros.cmake)

        # Needed when building bobuibase for android.
        if(ANDROID)
            include(src/corelib/BobUI6AndroidMacros.cmake)
            include(src/corelib/BobUI6AndroidDynamicFeatureHelpers.cmake)
            include(src/corelib/BobUI6AndroidGradleHelpers.cmake)
            include(src/corelib/BobUI6AndroidPermissionHelpers.cmake)
        endif()

        # Needed when building for WebAssembly.
        if(WASM)
            include(cmake/BobUIWasmHelpers.cmake)
            include(src/corelib/BobUI6WasmMacros.cmake)
        endif()

        include(src/tools/bobuiwaylandscanner/BobUI6WaylandCompositorMacros.cmake)

        ## Targets for global features, etc.:
        include(BobUIBaseGlobalTargets)

        ## Set language standards after BobUIBaseGlobalTargets, because that's when the relevant
        ## feature variables are available.
        bobui_set_language_standards()

        if(ANDROID)
            _bobui_internal_create_global_android_targets()
        endif()

        if(WASM)
            bobui_internal_setup_wasm_target_properties(Platform)
        endif()

        # Set up optimization flags like in qmake.
        # This function must be called after the global BOBUI_FEATURE_xxx variables have been set up,
        # aka after BobUIBaseGlobalTargets is processed.
        # It also has to be called /before/ adding add_subdirectory(src), so that per-directory
        # modifications can still be applied if necessary (like in done in Core and Gui).
        bobui_internal_set_up_config_optimizations_like_in_qmake()

        ## Setup documentation
        add_subdirectory(doc)

        ## Visit all the directories:
        add_subdirectory(src)
    endif()

    if(NOT BOBUI_INTERNAL_BUILD_STANDALONE_PARTS)
        if(BOBUI_FEATURE_settings)
            add_subdirectory(qmake)
        endif()

        bobui_internal_bobuibase_install_mkspecs()
        bobui_internal_bobuibase_install_wayland_files()
    endif()

    bobui_build_repo_post_process()

    bobui_build_repo_impl_tests()

    bobui_build_repo_end()

    bobui_build_repo_impl_examples()
endmacro()
