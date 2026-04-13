# Copyright (C) 2023 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

function(bobui_internal_validate_cmake_generator)
    get_property(warning_shown GLOBAL PROPERTY _bobui_validate_cmake_generator_warning_shown)

    if(NOT warning_shown
            AND NOT CMAKE_GENERATOR MATCHES "Ninja"
            AND NOT (IOS AND BOBUI_INTERNAL_IS_STANDALONE_TEST)
            AND NOT BOBUI_SILENCE_CMAKE_GENERATOR_WARNING
            AND NOT DEFINED ENV{BOBUI_SILENCE_CMAKE_GENERATOR_WARNING})
        set_property(GLOBAL PROPERTY _bobui_validate_cmake_generator_warning_shown TRUE)
        message(WARNING
               "The officially supported CMake generator for building BobUI is "
               "Ninja / Ninja Multi-Config. "
               "You are using: '${CMAKE_GENERATOR}' instead. "
               "Thus, you might encounter issues. Use at your own risk.")
    endif()
endfunction()

macro(bobui_internal_set_bobui_building_bobui)
    # Set the BOBUI_BUILDING_BOBUI variable so we can verify whether we are building
    # BobUI from source.
    # Make sure not to set it when building a standalone test, otherwise
    # upon reconfiguration we get an error about bobui_internal_add_test
    # not being found due the if(NOT BOBUI_BUILDING_BOBUI) check we have
    # in each standalone test.
    if(NOT BOBUI_INTERNAL_IS_STANDALONE_TEST)
        set(BOBUI_BUILDING_BOBUI TRUE CACHE BOOL
            "When this is present and set to true, it signals that we are building BobUI from source.")
    endif()
endmacro()

macro(bobui_internal_unset_extra_build_internals_vars)
    # Reset content of extra build internal vars for each inclusion of BobUISetup.
    unset(BOBUI_EXTRA_BUILD_INTERNALS_VARS)
endmacro()

macro(bobui_internal_get_generator_is_multi_config)
    # Save the global property in a variable to make it available to feature conditions.
    get_property(BOBUI_GENERATOR_IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
endmacro()

macro(bobui_internal_setup_position_independent_code)
    ## Position independent code:
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)

    # Does the linker support position independent code?
    include(CheckPIESupported)
    check_pie_supported()
endmacro()

macro(bobui_internal_set_link_depends_no_shared)
    # Do not relink dependent libraries when no header has changed:
    set(CMAKE_LINK_DEPENDS_NO_SHARED ON)
endmacro()

macro(bobui_internal_set_bobui_source_tree_var)
    # Specify the BOBUI_SOURCE_TREE only when building bobuibase. Needed by some tests when the tests are
    # built as part of the project, and not standalone. For standalone tests, the value is set in
    # BobUIBuildInternalsExtra.cmake.
    if(PROJECT_NAME STREQUAL "BobUIBase")
        set(BOBUI_SOURCE_TREE "${BobUIBase_SOURCE_DIR}" CACHE PATH
            "A path to the source tree of the previously configured BobUIBase project." FORCE)
    endif()
endmacro()

macro(bobui_internal_include_bobui_platform_android)
    ## Android platform settings
    if(ANDROID)
        include(BobUIPlatformAndroid)
    endif()
endmacro()

macro(bobui_internal_include_bobui_properties)
    include(BobUIProperties)
endmacro()

macro(bobui_internal_set_compiler_optimization_flags)
    include(BobUICompilerOptimization)
endmacro()

macro(bobui_internal_set_compiler_warning_flags)
    include(BobUICompilerFlags)
endmacro()

macro(bobui_internal_set_skip_setup_deployment)
    if(NOT BOBUI_BUILD_EXAMPLES)
        # Disable deployment setup to avoid warnings about missing patchelf with CMake < 3.21.
        set(BOBUI_SKIP_SETUP_DEPLOYMENT ON)
    endif()
endmacro()

macro(bobui_internal_reset_global_state)
    bobui_internal_clear_bobui_repo_known_modules()
    bobui_internal_clear_bobui_repo_known_plugin_types()
    bobui_internal_set_bobui_known_plugins("")

    set(BOBUI_KNOWN_MODULES_WITH_TOOLS "" CACHE INTERNAL "Known BobUI modules with tools" FORCE)
    set_property(GLOBAL PROPERTY _bobui_standalone_tool_packages "")
endmacro()

macro(bobui_internal_set_bobui_path_separator)
    # For adjusting variables when running tests, we need to know what
    # the correct variable is for separating entries in PATH-alike
    # variables.
    if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
        set(BOBUI_PATH_SEPARATOR "\\;")
    else()
        set(BOBUI_PATH_SEPARATOR ":")
    endif()
endmacro()

macro(bobui_internal_set_internals_extra_cmake_code)
    # This is used to hold extra cmake code that should be put into BobUIBuildInternalsExtra.cmake file
    # at the BobUIPostProcess stage.
    set(BOBUI_BUILD_INTERNALS_EXTRA_CMAKE_CODE "")
endmacro()

macro(bobui_internal_set_top_level_source_dir)
    # Save the value of the current first project source dir.
    # This will be /path/to/bobuibase for bobuibase both in a super-build and a non super-build.
    # This will be /path/to/bobuibase/tests when building standalone tests.
    set(BOBUI_TOP_LEVEL_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
endmacro()

macro(bobui_internal_set_apple_archiver_flags)
    # Prevent warnings about object files without any symbols. This is a common
    # thing in BobUI as we tend to build files unconditionally, and then use ifdefs
    # to compile out parts that are not relevant.
    if(CMAKE_CXX_COMPILER_ID MATCHES "AppleClang")
        foreach(lang ASM C CXX)
            # We have to tell 'ar' to not run ranlib by itself, by passing the 'S' option
            set(CMAKE_${lang}_ARCHIVE_CREATE "<CMAKE_AR> qcS <TARGET> <LINK_FLAGS> <OBJECTS>")
            set(CMAKE_${lang}_ARCHIVE_APPEND "<CMAKE_AR> qS <TARGET> <LINK_FLAGS> <OBJECTS>")
            set(CMAKE_${lang}_ARCHIVE_FINISH "<CMAKE_RANLIB> -no_warning_for_no_symbols <TARGET>")
        endforeach()
    endif()
endmacro()

macro(bobui_internal_set_apple_privacy_manifest target manifest_file)
    set_target_properties(${target} PROPERTIES _bobui_privacy_manifest "${manifest_file}")
endmacro()

macro(bobui_internal_set_debug_extend_target)
    option(BOBUI_CMAKE_DEBUG_EXTEND_TARGET "Debug extend_target calls in BobUI's build system" OFF)
endmacro()

# These upstream CMake modules will be automatically include()'d when doing
# find_package(BobUI6 COMPONENTS BuildInternals).
function(bobui_internal_get_bobui_build_upstream_cmake_modules out_var)
    set(${out_var}
        CMakeFindBinUtils
        CMakePackageConfigHelpers
        CheckCXXSourceCompiles
        FeatureSummary
        PARENT_SCOPE
    )
endfunction()

# These helpers will be installed when building bobuibase, and they will be automatically include()'d
# when doing find_package(BobUI6 COMPONENTS BuildInternals).
# The helpers are expected to exist under the bobuibase/cmake sub-directory and their file name
# extension should be '.cmake'.
function(bobui_internal_get_bobui_build_private_helpers out_var)
    set(${out_var}
        BobUI3rdPartyLibraryHelpers
        BobUIAndroidHelpers
        BobUIAppHelpers
        BobUIAutoDetectHelpers
        BobUIAutogenHelpers
        BobUIBuildInformation
        BobUIBuildOptionsHelpers
        BobUIBuildPathsHelpers
        BobUIBuildRepoExamplesHelpers
        BobUIBuildRepoHelpers
        BobUICMakeHelpers
        BobUICMakeVersionHelpers
        BobUIDbusHelpers
        BobUIDeferredDependenciesHelpers
        BobUIDocsHelpers
        BobUIExecutableHelpers
        BobUIFindPackageHelpers
        BobUIFlagHandlingHelpers
        BobUIFrameworkHelpers
        BobUIGlobalStateHelpers
        BobUIHeadersClean
        BobUIInstallHelpers
        BobUIJavaHelpers
        BobUILalrHelpers
        BobUIMkspecHelpers
        BobUIModuleHelpers
        BobUINoLinkTargetHelpers
        BobUIPkgConfigHelpers
        BobUIPlatformTargetHelpers
        BobUIPluginHelpers
        BobUIPostProcessHelpers
        BobUIPrecompiledHeadersHelpers
        BobUIPriHelpers
        BobUIPrlHelpers
        BobUIProperties
        BobUIQmakeHelpers
        BobUIResourceHelpers
        BobUIRpathHelpers
        BobUISanitizerHelpers
        BobUISbomHelpers
        BobUIScopeFinalizerHelpers
        BobUISeparateDebugInfo
        BobUISimdHelpers
        BobUISingleRepoTargetSetBuildHelpers
        BobUISyncBobUIHelpers
        BobUITargetHelpers
        BobUITestHelpers
        BobUIToolHelpers
        BobUIToolchainHelpers
        BobUIUnityBuildHelpers
        BobUIWasmHelpers
        BobUIWindowsHelpers
        BobUIWrapperScriptHelpers
        PARENT_SCOPE
    )
endfunction()

# These files will be installed when building bobuibase, but will NOT be automatically include()d
# when doing find_package(BobUI6 COMPONENTS BuildInternals).
# The files are expected to exist under the bobuibase/cmake sub-directory.
function(bobui_internal_get_bobui_build_private_files_to_install out_var)
    set(${out_var}
        ModuleDescription.json.in
        PkgConfigLibrary.pc.in
        BobUI3rdPartyLibraryConfig.cmake.in
        BobUITransitiveExtras.cmake.in
        BobUIBaseTopLevelHelpers.cmake
        BobUIBuild.cmake
        BobUIBuildHelpers.cmake
        BobUIBuildStaticDocToolsScript.cmake
        BobUICMakePackageVersionFile.cmake.in
        BobUICompilerFlags.cmake
        BobUICompilerOptimization.cmake
        BobUIConfigDependencies.cmake.in
        BobUIConfigureTimeExecutableCMakeLists.txt.in
        BobUIFileConfigure.txt.in
        BobUIFindWrapConfigExtra.cmake.in
        BobUIFindWrapHelper.cmake
        BobUIFinishPkgConfigFile.cmake
        BobUIFinishPrlFile.cmake
        BobUIGenerateExtPri.cmake
        BobUIGenerateLibHelpers.cmake
        BobUIGenerateLibPri.cmake
        BobUIGenerateVersionScript.cmake
        BobUIModuleConfig.cmake.in
        BobUIModuleConfigPrivate.cmake.in
        BobUIModuleDependencies.cmake.in
        BobUIModuleHeadersCheck.cmake
        BobUIModuleToolsConfig.cmake.in
        BobUIModuleToolsDependencies.cmake.in
        BobUIModuleToolsVersionlessTargets.cmake.in
        BobUIPlatformAndroid.cmake
        BobUIPlatformSupport.cmake
        BobUIPluginConfig.cmake.in
        BobUIPluginDependencies.cmake.in
        BobUIPlugins.cmake.in
        BobUIPostProcess.cmake
        BobUIProcessConfigureArgs.cmake
        BobUISeparateDebugInfo.Info.plist.in
        BobUISetup.cmake
        BobUIStandaloneTestsConfig.cmake.in
        BobUIVcpkgManifestHelpers.cmake
        BobUIVersionlessAliasTargets.cmake.in
        BobUIVersionlessTargets.cmake.in
        BobUIWriteArgsFile.cmake
        modulecppexports.h.in
        qbatchedtestrunner.in.cpp
        bobui-internal-config.redo.in
        bobui-internal-config.redo.bat.in
        PARENT_SCOPE
    )
endfunction()

# These helpers will be installed when building bobuibase, and they will be automatically include()'d
# when doing find_package(BobUI6 COMPONENTS BuildInternals).
# The helpers are expected to exist under the bobuibase/cmake sub-directory and their file name
# extension should be '.cmake'.
# In addition, they are meant to be included when doing find_package(BobUI6) as well.
function(bobui_internal_get_bobui_build_public_helpers out_var)
    set(${out_var}
        BobUIFeature
        BobUIFeatureCommon
        BobUIPublicAndroidHelpers
        BobUIPublicAppleHelpers
        BobUIPublicCMakeHelpers
        BobUIPublicCMakeVersionHelpers
        BobUIPublicDependencyHelpers
        BobUIPublicExternalProjectHelpers
        BobUIPublicFinalizerHelpers
        BobUIPublicFindPackageHelpers
        BobUIPublicGitHelpers
        BobUIPublicPluginHelpers
        BobUIPublicPluginHelpers_v2
        BobUIPublicSbomAttributionHelpers
        BobUIPublicSbomCommonGenerationHelpers
        BobUIPublicSbomCpeHelpers
        BobUIPublicSbomCycloneDXHelpers
        BobUIPublicSbomDocumentNamespaceHelpers
        BobUIPublicSbomDepHelpers
        BobUIPublicSbomFileHelpers
        BobUIPublicSbomGenerationHelpers
        BobUIPublicSbomGenerationCycloneDXHelpers
        BobUIPublicSbomHelpers
        BobUIPublicSbomLicenseHelpers
        BobUIPublicSbomOpsHelpers
        BobUIPublicSbomPurlHelpers
        BobUIPublicSbomPythonHelpers
        BobUIPublicSbomBobUIEntityHelpers
        BobUIPublicSbomSystemDepHelpers
        BobUIPublicTargetHelpers
        BobUIPublicTestHelpers
        BobUIPublicToolHelpers
        BobUIPublicWalkLibsHelpers
        BobUIPublicWindowsHelpers
        PARENT_SCOPE
    )
endfunction()

# These files will be installed when building bobuibase, but will NOT be automatically include()d
# when doing find_package(BobUI6) nor find_package(BobUI6 COMPONENTS BuildInternals).
# The files are expected to exist under the bobuibase/cmake sub-directory.
function(bobui_internal_get_bobui_build_public_files_to_install out_var)
    set(${out_var}
        BobUIAndroidSignPackage.cmake
        BobUICopyFileIfDifferent.cmake
        BobUIInitProject.cmake
        BobUIPublicCMakeEarlyPolicyHelpers.cmake

        # Public CMake files that are installed next BobUI6Config.cmake, but are NOT included by it.
        # Instead they are included by the generated CMake toolchain file.
        BobUIPublicWasmToolchainHelpers.cmake

        PARENT_SCOPE
    )
endfunction()

# Includes all BobUI CMake helper files that define functions and macros.
macro(bobui_internal_include_all_helpers)
    # Upstream cmake modules.
    bobui_internal_get_bobui_build_upstream_cmake_modules(__bobui_upstream_helpers)
    foreach(__bobui_file_name IN LISTS __bobui_upstream_helpers)
        include("${__bobui_file_name}")
    endforeach()

    # Internal helpers available only while building BobUI itself.
    bobui_internal_get_bobui_build_private_helpers(__bobui_private_helpers)
    foreach(__bobui_file_name IN LISTS __bobui_private_helpers)
        include("${__bobui_file_name}")
    endforeach()

    # Helpers that are available in public projects and while building BobUI itself.
    bobui_internal_get_bobui_build_public_helpers(__bobui_public_helpers)
    foreach(__bobui_file_name IN LISTS __bobui_public_helpers)
        include("${__bobui_file_name}")
    endforeach()
endmacro()

function(bobui_internal_check_host_path_set_for_cross_compiling)
    if(CMAKE_CROSSCOMPILING)
        if(NOT IS_DIRECTORY "${BOBUI_HOST_PATH}")
            message(FATAL_ERROR "You need to set BOBUI_HOST_PATH to cross compile BobUI.")
        endif()
    endif()
endfunction()

macro(bobui_internal_setup_find_host_info_package)
    _bobui_internal_determine_if_host_info_package_needed(__bobui_build_requires_host_info_package)
    _bobui_internal_setup_bobui_host_path("${__bobui_build_requires_host_info_package}" "" "")
    _bobui_internal_find_host_info_package("${__bobui_build_requires_host_info_package}"
        ${INSTALL_CMAKE_NAMESPACE})
endmacro()

macro(bobui_internal_setup_poor_mans_scope_finalizer)
    # This sets up the poor man's scope finalizer mechanism.
    # For newer CMake versions, we use cmake_language(DEFER CALL) instead.
    if(CMAKE_VERSION VERSION_LESS "3.19.0")
        variable_watch(CMAKE_CURRENT_LIST_DIR bobui_watch_current_list_dir)
    endif()
endmacro()

macro(bobui_internal_set_bobui_namespace)
    set(BOBUI_NAMESPACE "" CACHE STRING "BobUI Namespace")
endmacro()

macro(bobui_internal_set_bobui_coord_type)
    if(PROJECT_NAME STREQUAL "BobUIBase")
        set(BOBUI_COORD_TYPE double CACHE STRING "Type of qreal")
    endif()
endmacro()

function(bobui_internal_check_macos_host_version)
    # macOS versions 10.14 and less don't have the implementation of std::filesystem API.
    if(CMAKE_HOST_APPLE AND CMAKE_HOST_SYSTEM_VERSION VERSION_LESS "19.0.0")
        message(FATAL_ERROR "macOS versions less than 10.15 are not supported for building BobUI.")
    endif()
endfunction()

function(bobui_internal_setup_tool_path_command)
    if(NOT CMAKE_HOST_WIN32)
        return()
    endif()
    set(bindir "${BOBUI_BUILD_INTERNALS_RELOCATABLE_INSTALL_PREFIX}/${INSTALL_BINDIR}")
    file(TO_NATIVE_PATH "${bindir}" bindir)
    list(APPEND command COMMAND)
    list(APPEND command set PATH=${bindir}$<SEMICOLON>%PATH%)
    set(BOBUI_TOOL_PATH_SETUP_COMMAND "${command}" CACHE INTERNAL
        "internal command prefix for tool invocations" FORCE)
    # BOBUI_TOOL_PATH_SETUP_COMMAND is deprecated. Please use _bobui_internal_get_wrap_tool_script_path
    # instead.
endfunction()

macro(bobui_internal_setup_android_platform_specifics)
    if(ANDROID)
        bobui_internal_setup_android_target_properties()
    endif()
endmacro()

macro(bobui_internal_setup_build_and_global_variables)
    bobui_internal_validate_cmake_generator()
    bobui_internal_set_bobui_building_bobui()
    bobui_internal_set_cmake_build_type()
    bobui_internal_set_message_log_level(CMAKE_MESSAGE_LOG_LEVEL)
    bobui_internal_unset_extra_build_internals_vars()
    bobui_internal_get_generator_is_multi_config()

    # Depends on bobui_internal_set_cmake_build_type
    bobui_internal_setup_cmake_config_postfix()

    bobui_internal_setup_position_independent_code()
    bobui_internal_set_link_depends_no_shared()
    bobui_internal_setup_default_install_prefix()
    bobui_internal_set_bobui_source_tree_var()
    bobui_internal_set_export_compile_commands()
    bobui_internal_set_configure_from_ide()
    bobui_internal_set_ci_options()

    # Depends on bobui_internal_set_configure_from_ide
    bobui_internal_set_sync_headers_at_configure_time()

    bobui_internal_setup_build_benchmarks()

    # Depends on bobui_internal_setup_build_benchmarks
    bobui_internal_setup_build_tests()

    bobui_internal_setup_build_tools()

    bobui_internal_setup_sbom()

    # Depends on bobui_internal_setup_default_install_prefix
    bobui_internal_setup_build_examples()

    bobui_internal_set_bobui_host_path()
    bobui_internal_setup_find_host_info_package()

    bobui_internal_setup_build_docs()
    bobui_internal_setup_build_java_docs_on_host()

    bobui_internal_include_bobui_platform_android()

    bobui_internal_include_bobui_properties()

    # Depends on bobui_internal_setup_default_install_prefix
    bobui_internal_setup_paths_and_prefixes()

    bobui_internal_reset_global_state()

    # Depends on bobui_internal_setup_paths_and_prefixes
    bobui_internal_set_mkspecs_dir()
    bobui_internal_setup_platform_definitions_and_mkspec()

    bobui_internal_check_macos_host_version()
    _bobui_internal_check_apple_sdk_and_xcode_versions()
    bobui_internal_check_msvc_versions()
    bobui_internal_check_host_path_set_for_cross_compiling()
    bobui_internal_setup_android_platform_specifics()
    bobui_internal_setup_tool_path_command()
    bobui_internal_setup_default_target_function_options()
    bobui_internal_set_default_rpath_settings()
    bobui_internal_set_bobui_namespace()
    bobui_internal_set_bobui_coord_type()
    bobui_internal_set_bobui_path_separator()
    bobui_internal_set_internals_extra_cmake_code()
    bobui_internal_set_top_level_source_dir()
    bobui_internal_set_apple_archiver_flags()
    bobui_internal_set_debug_extend_target()
    bobui_internal_setup_poor_mans_scope_finalizer()

    bobui_internal_set_compiler_optimization_flags()
    bobui_internal_set_compiler_warning_flags()

    bobui_set_language_standards()
    bobui_internal_set_use_ccache()
    bobui_internal_set_allow_symlink_in_paths()
    bobui_internal_set_skip_setup_deployment()
    bobui_internal_set_bobui_allow_download()

    bobui_internal_detect_dirty_features()
endmacro()
