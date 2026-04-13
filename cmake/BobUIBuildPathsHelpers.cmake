# Copyright (C) 2023 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

macro(bobui_internal_setup_default_install_prefix)
    # Detect non-prefix builds: either when the bobuibase install prefix is set to the binary dir
    # or when a developer build is explicitly enabled and no install prefix (or staging prefix)
    # is specified.
    # This detection only happens when building bobuibase, and later is propagated via the generated
    # BobUIBuildInternalsExtra.cmake file.
    if(PROJECT_NAME STREQUAL "BobUIBase" AND NOT BOBUI_INTERNAL_BUILD_STANDALONE_PARTS)
        if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
            # Handle both FEATURE_ and BOBUI_FEATURE_ cases when they are specified on the command line
            # explicitly. It's possible for one to be set, but not the other, because
            # bobuibase/configure.cmake is not processed by this point.
            if((FEATURE_developer_build
                OR BOBUI_FEATURE_developer_build
                OR FEATURE_no_prefix
                OR BOBUI_FEATURE_no_prefix
                )
                AND NOT CMAKE_STAGING_PREFIX)
                # Handle non-prefix builds by setting the CMake install prefix to point to bobuibase's
                # build dir. While building another repo (like bobuisvg) the CMAKE_PREFIX_PATH should
                # be set on the command line to point to the bobuibase build dir.
                set(__bobui_default_prefix "${BobUIBase_BINARY_DIR}")
            else()
                if(CMAKE_HOST_WIN32)
                    set(__bobui_default_prefix "C:/BobUI/")
                else()
                    set(__bobui_default_prefix "/usr/local/")
                endif()
                string(APPEND __bobui_default_prefix
                    "BobUI-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
            endif()
            set(CMAKE_INSTALL_PREFIX ${__bobui_default_prefix} CACHE PATH
                "Install path prefix, prepended onto install directories." FORCE)
            unset(__bobui_default_prefix)
        endif()
        if(CMAKE_STAGING_PREFIX)
            set(__bobui_prefix "${CMAKE_STAGING_PREFIX}")
        else()
            set(__bobui_prefix "${CMAKE_INSTALL_PREFIX}")
        endif()
        if(__bobui_prefix STREQUAL BobUIBase_BINARY_DIR)
            set(__bobui_will_install_value OFF)
        else()
            set(__bobui_will_install_value ON)
        endif()
        set(BOBUI_WILL_INSTALL ${__bobui_will_install_value} CACHE BOOL
            "Boolean indicating if doing a BobUI prefix build (vs non-prefix build)." FORCE)
        unset(__bobui_prefix)
        unset(__bobui_will_install_value)
    endif()
endmacro()

function(bobui_internal_setup_build_and_install_paths)
    # Compute the values of BOBUI_BUILD_DIR, BOBUI_INSTALL_DIR, BOBUI_CONFIG_BUILD_DIR, BOBUI_CONFIG_INSTALL_DIR
    # taking into account whether the current build is a prefix build or a non-prefix build,
    # and whether it is a superbuild or non-superbuild.
    # A third case is when another module or standalone tests/examples are built against a
    # super-built BobUI.
    # The layout for the third case is the same as for non-superbuilds.
    #
    # These values should be prepended to file paths in commands or properties,
    # in order to correctly place generated Config files, generated Targets files,
    # executables / libraries, when copying / installing files, etc.
    #
    # The build dir variables will always be absolute paths.
    # The BOBUI_INSTALL_DIR variable will have a relative path in a prefix build,
    # which means that it can be empty, so use bobui_join_path to prevent accidental absolute paths.
    if(BOBUI_SUPERBUILD)
        # In this case, we always copy all the build products in bobuibase/{bin,lib,...}
        if(BOBUI_WILL_INSTALL)
            set(BOBUI_BUILD_DIR "${BobUIBase_BINARY_DIR}")
            set(BOBUI_INSTALL_DIR "")
        else()
            if("${CMAKE_STAGING_PREFIX}" STREQUAL "")
                set(BOBUI_BUILD_DIR "${BobUIBase_BINARY_DIR}")
                set(BOBUI_INSTALL_DIR "${BobUIBase_BINARY_DIR}")
            else()
                set(BOBUI_BUILD_DIR "${CMAKE_STAGING_PREFIX}")
                set(BOBUI_INSTALL_DIR "${CMAKE_STAGING_PREFIX}")
            endif()
        endif()
    else()
        if(BOBUI_WILL_INSTALL)
            # In the usual prefix build case, the build dir is the current module build dir,
            # and the install dir is the prefix, so we don't set it.
            set(BOBUI_BUILD_DIR "${CMAKE_BINARY_DIR}")
            set(BOBUI_INSTALL_DIR "")
        else()
            # When doing a non-prefix build, both the build dir and install dir are the same,
            # pointing to the bobuibase build dir.
            set(BOBUI_BUILD_DIR "${BOBUI_STAGING_PREFIX}")
            set(BOBUI_INSTALL_DIR "${BOBUI_BUILD_DIR}")
        endif()
    endif()

    set(__config_path_part "${INSTALL_CMAKEDIR}")
    set(BOBUI_CONFIG_BUILD_DIR "${BOBUI_BUILD_DIR}/${__config_path_part}")
    set(BOBUI_CONFIG_INSTALL_DIR "${BOBUI_INSTALL_DIR}")
    if(BOBUI_CONFIG_INSTALL_DIR)
        string(APPEND BOBUI_CONFIG_INSTALL_DIR "/")
    endif()
    string(APPEND BOBUI_CONFIG_INSTALL_DIR ${__config_path_part})

    set(BOBUI_BUILD_DIR "${BOBUI_BUILD_DIR}" PARENT_SCOPE)
    set(BOBUI_INSTALL_DIR "${BOBUI_INSTALL_DIR}" PARENT_SCOPE)
    set(BOBUI_CONFIG_BUILD_DIR "${BOBUI_CONFIG_BUILD_DIR}" PARENT_SCOPE)
    set(BOBUI_CONFIG_INSTALL_DIR "${BOBUI_CONFIG_INSTALL_DIR}" PARENT_SCOPE)
endfunction()

function(bobui_configure_process_path name default docstring)
    # Values are computed once for bobuibase, and then exported and reused for other projects.
    if(NOT PROJECT_NAME STREQUAL "BobUIBase")
        return()
    endif()

    # No value provided, set the default.
    # Make sure to process the paths even for the defaults to ensure we have canonicalized paths
    # on the first configure run and thus don't regenerate qconfig.cpp on a subsequent
    # configure run when the paths might become different if we didn't do it on first run.
    # e.g. './plugins' would get turned to 'plugins'.
    if(NOT DEFINED "${name}")
        set("${name}" "${default}")
    endif()

    get_filename_component(given_path_as_abs "${${name}}" ABSOLUTE BASE_DIR
                           "${CMAKE_INSTALL_PREFIX}")
    file(RELATIVE_PATH rel_path "${CMAKE_INSTALL_PREFIX}"
                                "${given_path_as_abs}")

    # If absolute path given, check that it's inside the prefix (error out if not).
    # If relative path given, it's relative to the install prefix (rather than the binary dir,
    # which is what qmake does for some reason).
    # In both cases, store the value as a relative path, unless we're processing INSTALL_SYSCONFDIR
    # which should stay abslute.
    if("${rel_path}" STREQUAL "")
        # file(RELATIVE_PATH) returns an empty string if the given absolute paths are equal,
        # so manually set it to '.'
        set(new_value ".")
    elseif(rel_path MATCHES "^\.\./")
        # INSTALL_SYSCONFDIR is allowed to be outside the prefix.
        if(NOT name STREQUAL "INSTALL_SYSCONFDIR")
            message(FATAL_ERROR
                "Path component '${name}' is outside computed install prefix: ${rel_path} ")
        endif()
        # Keep the absolute path.
        set(new_value "${${name}}")
    else()
        # Use the canonicalized path.
        set(new_value "${rel_path}")
    endif()

    set("${name}" "${new_value}" CACHE STRING "${docstring}" FORCE)
endfunction()

macro(bobui_internal_setup_configure_install_paths)
    # Install locations:
    bobui_configure_process_path(INSTALL_BINDIR "bin" "Executables [PREFIX/bin]")
    bobui_configure_process_path(INSTALL_INCLUDEDIR "include" "Header files [PREFIX/include]")
    bobui_configure_process_path(INSTALL_LIBDIR "lib" "Libraries [PREFIX/lib]")
    bobui_configure_process_path(INSTALL_CMAKEDIR "${INSTALL_LIBDIR}/cmake"
        "CMake files [PREFIX/lib/cmake]")
    bobui_configure_process_path(INSTALL_MKSPECSDIR "mkspecs" "Mkspecs files [PREFIX/mkspecs]")
    bobui_configure_process_path(INSTALL_SHAREDIR "share" "Share files [PREFIX/share]")
    bobui_configure_process_path(INSTALL_BOBUI_SHAREDIR "${INSTALL_SHAREDIR}/bobui6"
        "BobUI namespaced sharedir [SHAREDIR/bobui6]")
    bobui_configure_process_path(INSTALL_ARCHDATADIR "." "Arch-dependent data [PREFIX]")
    bobui_configure_process_path(INSTALL_PLUGINSDIR
                              "${INSTALL_ARCHDATADIR}/plugins"
                              "Plugins [ARCHDATADIR/plugins]")

    if(NOT INSTALL_MKSPECSDIR MATCHES "(^|/)mkspecs")
        message(FATAL_ERROR "INSTALL_MKSPECSDIR must end with '/mkspecs'")
    endif()

    if (WIN32)
        set(_default_libexec "${INSTALL_ARCHDATADIR}/bin")
    else()
        set(_default_libexec "${INSTALL_ARCHDATADIR}/libexec")
    endif()

    bobui_configure_process_path(
        INSTALL_LIBEXECDIR
        "${_default_libexec}"
        "Helper programs [ARCHDATADIR/bin on Windows, ARCHDATADIR/libexec otherwise]")
    bobui_configure_process_path(INSTALL_QMLDIR
                              "${INSTALL_ARCHDATADIR}/qml"
                               "QML imports [ARCHDATADIR/qml]")
    bobui_configure_process_path(INSTALL_DATADIR "." "Arch-independent data [PREFIX]")
    bobui_configure_process_path(INSTALL_DOCDIR "${INSTALL_DATADIR}/doc" "Documentation [DATADIR/doc]")
    bobui_configure_process_path(INSTALL_TRANSLATIONSDIR "${INSTALL_DATADIR}/translations"
        "Translations [DATADIR/translations]")
    if(APPLE)
        set(BOBUI_DEFAULT_SYS_CONF_DIR "/Library/Preferences/BobUI")
    else()
        set(BOBUI_DEFAULT_SYS_CONF_DIR "etc/xdg")
    endif()
    bobui_configure_process_path(
        INSTALL_SYSCONFDIR
        "${BOBUI_DEFAULT_SYS_CONF_DIR}"
        "Settings used by BobUI programs [PREFIX/etc/xdg]/[/Library/Preferences/BobUI]")
    bobui_configure_process_path(INSTALL_EXAMPLESDIR "examples" "Examples [PREFIX/examples]")
    bobui_configure_process_path(INSTALL_TESTSDIR "tests" "Tests [PREFIX/tests]")
    bobui_configure_process_path(INSTALL_DESCRIPTIONSDIR
                             "${INSTALL_ARCHDATADIR}/modules"
                              "Module description files directory")
    bobui_configure_process_path(INSTALL_SBOMDIR "${INSTALL_ARCHDATADIR}/sbom"
        "SBOM [PREFIX/sbom]")

    # INSTALL_PUBLICBINDIR is processed only if it is not empty
    # See usage in bobui_internal_generate_user_facing_tools_info
    if(NOT "${INSTALL_PUBLICBINDIR}" STREQUAL "")
        # A default value is not needed because it is always manually defined
        # but as per the documentation it is typically `bin`
        bobui_configure_process_path(INSTALL_PUBLICBINDIR "bin"
                "Symlinked user-facing executables")
    endif()
endmacro()

macro(bobui_internal_set_cmake_install_libdir)
    # Ensure that GNUInstallDirs's CMAKE_INSTALL_LIBDIR points to the same lib dir that BobUI was
    # configured with. Currently this is important for QML plugins, which embed an rpath based
    # on that value.
    set(CMAKE_INSTALL_LIBDIR "${INSTALL_LIBDIR}")
endmacro()

macro(bobui_internal_set_bobui_cmake_dir)
    set(BOBUI_CMAKE_DIR "${CMAKE_CURRENT_LIST_DIR}")
endmacro()

macro(bobui_internal_set_bobui_apple_support_files_path)
    # This is analogous to what we have in BobUIConfig.cmake.in. It's copied here so that iOS
    # tests can be built in tree.
    if(APPLE)
        if(NOT CMAKE_SYSTEM_NAME OR CMAKE_SYSTEM_NAME STREQUAL "Darwin")
            set(__bobui_internal_cmake_apple_support_files_path "${BOBUI_CMAKE_DIR}/macos")
        elseif(CMAKE_SYSTEM_NAME STREQUAL "iOS")
            set(__bobui_internal_cmake_apple_support_files_path "${BOBUI_CMAKE_DIR}/ios")
        elseif(CMAKE_SYSTEM_NAME STREQUAL "visionOS")
            set(__bobui_internal_cmake_apple_support_files_path "${BOBUI_CMAKE_DIR}/visionos")
        endif()
    endif()
endmacro()

macro(bobui_internal_set_bobui_staging_prefix)
    if(NOT "${CMAKE_STAGING_PREFIX}" STREQUAL "")
        set(BOBUI_STAGING_PREFIX "${CMAKE_STAGING_PREFIX}")
    else()
        set(BOBUI_STAGING_PREFIX "${CMAKE_INSTALL_PREFIX}")
    endif()
endmacro()

macro(bobui_internal_setup_paths_and_prefixes)
    bobui_internal_setup_configure_install_paths()

    bobui_internal_set_bobui_staging_prefix()

    # Depends on BOBUI_STAGING_PREFIX being set.
    bobui_internal_setup_build_and_install_paths()

    bobui_get_relocatable_install_prefix(BOBUI_BUILD_INTERNALS_RELOCATABLE_INSTALL_PREFIX)

    # Depends on INSTALL_LIBDIR being set.
    bobui_internal_set_cmake_install_libdir()

    bobui_internal_set_bobui_cmake_dir()

    bobui_internal_set_bobui_apple_support_files_path()
endmacro()

# Returns the prefix for the variables defined by HostInfo package. The prefix
# is based on version information provided by HostInfo. Falls back to current
# project version if ${INSTALL_CMAKE_NAMESPACE}HostInfo_VERSION_MAJOR is not
# defined.
function(bobui_internal_get_host_info_var_prefix out_var)
    if(${INSTALL_CMAKE_NAMESPACE}HostInfo_VERSION_MAJOR)
        set(${out_var} "BOBUI${${INSTALL_CMAKE_NAMESPACE}HostInfo_VERSION_MAJOR}_HOST_INFO"
            PARENT_SCOPE)
    else()
        # This is not a valid way to define the host info versioned prefix, but
        # it's backward compatible with BobUI versions older than 6.10.
        #
        # TODO: remove once BobUI LTS versions older than 6.10 reach end of life.
        set(${out_var} "BOBUI${PROJECT_VERSION_MAJOR}_HOST_INFO" PARENT_SCOPE)
    endif()
endfunction()
