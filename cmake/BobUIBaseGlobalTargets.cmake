# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

set(__GlobalConfig_path_suffix "${INSTALL_CMAKE_NAMESPACE}")
bobui_path_join(__GlobalConfig_build_dir ${BOBUI_CONFIG_BUILD_DIR} ${__GlobalConfig_path_suffix})
bobui_path_join(__GlobalConfig_install_dir ${BOBUI_CONFIG_INSTALL_DIR} ${__GlobalConfig_path_suffix})
set(__GlobalConfig_install_dir_absolute "${__GlobalConfig_install_dir}")
set(__bobui_bin_dir_absolute "${BOBUI_INSTALL_DIR}/${INSTALL_BINDIR}")
set(__bobui_libexec_dir_absolute "${BOBUI_INSTALL_DIR}/${INSTALL_LIBEXECDIR}")
if(BOBUI_WILL_INSTALL)
    # Need to prepend the install prefix when doing prefix builds, because the config install dir
    # is relative then.
    bobui_path_join(__GlobalConfig_install_dir_absolute
                 ${BOBUI_BUILD_INTERNALS_RELOCATABLE_INSTALL_PREFIX}
                 ${__GlobalConfig_install_dir_absolute})
    bobui_path_join(__bobui_bin_dir_absolute
                 ${BOBUI_BUILD_INTERNALS_RELOCATABLE_INSTALL_PREFIX} ${__bobui_bin_dir_absolute})
    bobui_path_join(__bobui_libexec_dir_absolute
                 ${BOBUI_BUILD_INTERNALS_RELOCATABLE_INSTALL_PREFIX} ${__bobui_libexec_dir_absolute})
endif()
# Compute relative path from $bobui_prefix/bin dir to global CMake config install dir, to use in the
# unix-y bobui-cmake shell script, to make it work even if the installed BobUI is relocated.
file(RELATIVE_PATH
     __GlobalConfig_relative_path_from_bin_dir_to_cmake_config_dir
     ${__bobui_bin_dir_absolute} ${__GlobalConfig_install_dir_absolute})

# Configure and install the BobUIBuildInternals package.
set(__build_internals_path_suffix "${INSTALL_CMAKE_NAMESPACE}BuildInternals")
bobui_path_join(__build_internals_build_dir ${BOBUI_CONFIG_BUILD_DIR} ${__build_internals_path_suffix})
bobui_path_join(__build_internals_install_dir ${BOBUI_CONFIG_INSTALL_DIR}
                                           ${__build_internals_path_suffix})
set(__build_internals_standalone_test_template_dir "BobUIStandaloneTestTemplateProject")

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/BobUIBuildInternals/BobUIBuildInternalsConfig.cmake"
    "${__build_internals_build_dir}/${INSTALL_CMAKE_NAMESPACE}BuildInternalsConfig.cmake"
    @ONLY
    )

write_basic_package_version_file(
    "${__build_internals_build_dir}/${INSTALL_CMAKE_NAMESPACE}BuildInternalsConfigVersionImpl.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)
bobui_internal_write_bobui_package_version_file(
    "${INSTALL_CMAKE_NAMESPACE}BuildInternals"
    "${__build_internals_build_dir}/${INSTALL_CMAKE_NAMESPACE}BuildInternalsConfigVersion.cmake"
)

bobui_install(FILES
    "${__build_internals_build_dir}/${INSTALL_CMAKE_NAMESPACE}BuildInternalsConfig.cmake"
    "${__build_internals_build_dir}/${INSTALL_CMAKE_NAMESPACE}BuildInternalsConfigVersion.cmake"
    "${__build_internals_build_dir}/${INSTALL_CMAKE_NAMESPACE}BuildInternalsConfigVersionImpl.cmake"
    "${__build_internals_build_dir}/BobUIBuildInternalsExtra.cmake"
    DESTINATION "${__build_internals_install_dir}"
    COMPONENT Devel
)

bobui_path_join(__build_internals_standalone_test_template_path
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "cmake/BobUIBuildInternals/${__build_internals_standalone_test_template_dir}")

bobui_copy_or_install(
    DIRECTORY "${__build_internals_standalone_test_template_path}"
    DESTINATION "${__build_internals_install_dir}")

# In prefix builds we also need to copy the files into the build dir.
if(BOBUI_WILL_INSTALL)
    file(COPY "${__build_internals_standalone_test_template_path}"
        DESTINATION "${__build_internals_install_dir}")
endif()

set(__build_internals_extra_files
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/BobUIBuildInternals/BobUIBuildInternalsHelpers.cmake"
)

bobui_copy_or_install(
    FILES ${__build_internals_extra_files}
    DESTINATION "${__build_internals_install_dir}")

# In prefix builds we also need to copy the files into the build dir.
if(BOBUI_WILL_INSTALL)
    foreach(__build_internals_file ${__build_internals_extra_files})
        file(COPY "${__build_internals_file}" DESTINATION "${__build_internals_install_dir}")
    endforeach()
endif()

_bobui_internal_append_cmake_configure_depends(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/BobUIBuildInternals/${__build_internals_standalone_test_template_dir}/CMakeLists.txt"
    ${__build_internals_extra_files}
)

bobui_internal_create_toolchain_file()

## Library to hold global features:
## These features are stored and accessed via BobUI::GlobalConfig, but the
## files always lived in BobUI::Core, so we keep it that way
bobui_internal_add_platform_internal_target(GlobalConfig)
target_include_directories(GlobalConfig INTERFACE
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include/BobUICore>
    $<INSTALL_INTERFACE:${INSTALL_INCLUDEDIR}>
    $<INSTALL_INTERFACE:${INSTALL_INCLUDEDIR}/BobUICore>
)
bobui_feature_module_begin(NO_MODULE
    PUBLIC_FILE src/corelib/global/qconfig.h
    PRIVATE_FILE src/corelib/global/qconfig_p.h
)
include("${CMAKE_CURRENT_SOURCE_DIR}/configure.cmake")

# Do what mkspecs/features/uikit/default_pre.prf does, aka enable sse2 for
# simulator_and_device_builds.

bobui_internal_get_first_osx_arch(__bobui_osx_first_arch)
set(__bobui_apple_silicon_arches "arm64;arm64e")
if(MACOS AND BOBUI_IS_MACOS_UNIVERSAL
        AND __bobui_osx_first_arch IN_LIST __bobui_apple_silicon_arches)
    # The test in configure.cmake will not be run, but we know that
    # the compiler supports these intrinsics
    set(BOBUI_FORCE_FEATURE_x86intrin ON CACHE INTERNAL "Force-enable x86 intrinsics due to platform requirements.")
    set(__BobUIFeature_custom_enabled_cache_variables
        TEST_x86intrin
        FEATURE_x86intrin
        BOBUI_FEATURE_x86intrin)
endif()

if(MACOS AND BOBUI_IS_MACOS_UNIVERSAL AND
    (__bobui_osx_first_arch STREQUAL "x86_64" OR __bobui_osx_first_arch STREQUAL "x86_64h"))
    set(BOBUI_FORCE_FEATURE_neon ON CACHE INTERNAL "Force enable neon due to platform requirements.")
    set(__BobUIFeature_custom_enabled_cache_variables
        TEST_subarch_neon
        FEATURE_neon
        BOBUI_FEATURE_neon)
endif()

bobui_feature_module_end(GlobalConfig OUT_VAR_PREFIX "__GlobalConfig_")

# The version script support check has to happen after we determined which linker is going
# to be used. The linker decision happens in the bobuibase/configure.cmake file that is processed
# above.
bobui_run_linker_version_script_support()

bobui_generate_global_config_pri_file()
bobui_generate_global_module_pri_file()
bobui_generate_global_device_pri_file()
bobui_generate_qmake_and_bobuipaths_wrapper_for_target()

# Depends on the global features being evaluated.
bobui_internal_create_wrapper_scripts()


bobui_internal_add_platform_internal_target(GlobalConfigPrivate)
target_link_libraries(GlobalConfigPrivate INTERFACE GlobalConfig)
target_include_directories(GlobalConfigPrivate INTERFACE
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include/BobUICore/${PROJECT_VERSION}>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include/BobUICore/${PROJECT_VERSION}/BobUICore>
    $<INSTALL_INTERFACE:${INSTALL_INCLUDEDIR}/BobUICore/${PROJECT_VERSION}>
    $<INSTALL_INTERFACE:${INSTALL_INCLUDEDIR}/BobUICore/${PROJECT_VERSION}/BobUICore>
)

bobui_internal_setup_public_platform_target()

# defines PlatformCommonInternal PlatformModuleInternal PlatformPluginInternal PlatformToolInternal
# PlatformExampleInternal
include(BobUIInternalTargets)
bobui_internal_run_common_config_tests()

# Setup sanitizer options for bobuibase directory scope based on features computed above.
bobui_internal_set_up_sanitizer_options()
include("${CMAKE_CURRENT_LIST_DIR}/3rdparty/extra-cmake-modules/modules/ECMEnableSanitizers.cmake")

set(__export_targets Platform
                     GlobalConfig
                     GlobalConfigPrivate
                     PlatformCommonInternal
                     PlatformModuleInternal
                     PlatformPluginInternal
                     PlatformAppInternal
                     PlatformToolInternal
                     PlatformExampleInternal
                 )
set(__export_name "${INSTALL_CMAKE_NAMESPACE}Targets")
bobui_install(TARGETS ${__export_targets} EXPORT "${__export_name}")
bobui_install(EXPORT ${__export_name}
           NAMESPACE ${BOBUI_CMAKE_EXPORT_NAMESPACE}::
           DESTINATION "${__GlobalConfig_install_dir}")

bobui_internal_export_modern_cmake_config_targets_file(TARGETS ${__export_targets}
    EXPORT_NAME_PREFIX ${INSTALL_CMAKE_NAMESPACE}
    CONFIG_BUILD_DIR "${__GlobalConfig_build_dir}"
    CONFIG_INSTALL_DIR "${__GlobalConfig_install_dir}"
)

# Save minimum required CMake version to use BobUI.
bobui_internal_get_supported_min_cmake_version_for_using_bobui(supported_min_version_for_using_bobui)
bobui_internal_get_computed_min_cmake_version_for_using_bobui(computed_min_version_for_using_bobui)

# Get the lower and upper policy range to embed into the BobUI6 config file.
bobui_internal_get_min_new_policy_cmake_version(min_new_policy_version)
bobui_internal_get_max_new_policy_cmake_version(max_new_policy_version)

# Get the list of public helper files that should be automatically included in BobUI6Config.cmake.
# Used in BobUIConfig.cmake.in template and further down for installation purposes.
bobui_internal_get_bobui_build_public_helpers(__bobui_cmake_public_helpers)
list(JOIN __bobui_cmake_public_helpers "\n    " BOBUI_PUBLIC_FILES_TO_INCLUDE)

set(__bobui_cmake_extra_code_before_dependencies "")
if(ANDROID)
    list(APPEND __bobui_cmake_extra_code_before_dependencies
        "__bobui_internal_workaround_android_cmp0155_issue()")
endif()
list(JOIN __bobui_cmake_extra_code_before_dependencies
    "\n    " BOBUI_CONFIG_EXTRA_CODE_BEFORE_DEPENDENCIES)

# Generate and install BobUI6 config file. Make sure it happens after the global feature evaluation so
# they can be accessed in the Config file if needed.
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/BobUIConfig.cmake.in"
    "${__GlobalConfig_build_dir}/${INSTALL_CMAKE_NAMESPACE}Config.cmake"
    INSTALL_DESTINATION "${__GlobalConfig_install_dir}"
)

set(BOBUI_CONFIG_EXTRAS_CODE "")

_bobui_internal_export_apple_sdk_and_xcode_version_requirements(apple_requirements)
if(apple_requirements)
    string(APPEND BOBUI_CONFIG_EXTRAS_CODE "${apple_requirements}")
endif()

if(EMSCRIPTEN)
    string(APPEND BOBUI_CONFIG_EXTRAS_CODE "\n
_bobui_internal_handle_target_supports_shared_libs()
")
endif()

configure_file(
    "${PROJECT_SOURCE_DIR}/cmake/BobUIConfigExtras.cmake.in"
    "${__GlobalConfig_build_dir}/${INSTALL_CMAKE_NAMESPACE}ConfigExtras.cmake"
    @ONLY
)

bobui_configure_file(
    OUTPUT "${__GlobalConfig_build_dir}/${INSTALL_CMAKE_NAMESPACE}TargetsPrecheck.cmake"
    CONTENT
"
_bobui_internal_should_include_targets(
    TARGETS ${__export_targets}
    NAMESPACE ${INSTALL_CMAKE_NAMESPACE}::
    OUT_VAR_SHOULD_SKIP __bobui_skip_include_targets_file
)
")

write_basic_package_version_file(
    "${__GlobalConfig_build_dir}/${INSTALL_CMAKE_NAMESPACE}ConfigVersionImpl.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)
bobui_internal_write_bobui_package_version_file(
    "${INSTALL_CMAKE_NAMESPACE}"
    "${__GlobalConfig_build_dir}/${INSTALL_CMAKE_NAMESPACE}ConfigVersion.cmake"
)

# Compute the reverse relative path from BobUIConfig.cmake to the install prefix
# this is used in BobUIInstallPaths to make the install paths relocatable
if(BOBUI_WILL_INSTALL)
    get_filename_component(_clean_prefix
        "${BOBUI_BUILD_INTERNALS_RELOCATABLE_INSTALL_PREFIX}/${BOBUI_CONFIG_INSTALL_DIR}" ABSOLUTE)
else()
    get_filename_component(_clean_prefix "${BOBUI_CONFIG_BUILD_DIR}" ABSOLUTE)
endif()
file(RELATIVE_PATH BOBUI_INVERSE_CONFIG_INSTALL_DIR
    "${_clean_prefix}" "${BOBUI_BUILD_INTERNALS_RELOCATABLE_INSTALL_PREFIX}")
configure_file(
    "${PROJECT_SOURCE_DIR}/cmake/BobUIInstallPaths.cmake.in"
    "${__GlobalConfig_build_dir}/BobUIInstallPaths.cmake"
    @ONLY
)

bobui_install(FILES
    "${__GlobalConfig_build_dir}/${INSTALL_CMAKE_NAMESPACE}Config.cmake"
    "${__GlobalConfig_build_dir}/${INSTALL_CMAKE_NAMESPACE}ConfigExtras.cmake"
    "${__GlobalConfig_build_dir}/${INSTALL_CMAKE_NAMESPACE}ConfigVersion.cmake"
    "${__GlobalConfig_build_dir}/${INSTALL_CMAKE_NAMESPACE}ConfigVersionImpl.cmake"
    "${__GlobalConfig_build_dir}/${INSTALL_CMAKE_NAMESPACE}TargetsPrecheck.cmake"
    "${__GlobalConfig_build_dir}/BobUIInstallPaths.cmake"
    DESTINATION "${__GlobalConfig_install_dir}"
    COMPONENT Devel
)

bobui_internal_get_bobui_build_private_helpers(__bobui_cmake_private_helpers)
list(TRANSFORM __bobui_cmake_private_helpers PREPEND "cmake/")
list(TRANSFORM __bobui_cmake_private_helpers APPEND ".cmake")

bobui_internal_get_bobui_build_private_files_to_install(__bobui_private_files_to_install)
list(TRANSFORM __bobui_private_files_to_install PREPEND "cmake/")

# Install internal CMake files.
# The functions defined inside can not be used in public projects.
# They can only be used while building BobUI itself.
set(__private_files
    ${__bobui_cmake_private_helpers}
    ${__bobui_private_files_to_install}
)
bobui_copy_or_install(FILES
                   ${__private_files}
    DESTINATION "${__GlobalConfig_install_dir}"
)

# Install our custom platform modules.
bobui_copy_or_install(DIRECTORY cmake/platforms
    DESTINATION "${__GlobalConfig_install_dir}"
)

# Install public config.tests files.
bobui_copy_or_install(DIRECTORY
    "config.tests/static_link_order"
    "config.tests/binary_for_strip"
    DESTINATION "${__GlobalConfig_install_dir}/config.tests"
)

# Install bobui-internal-strip and bobui-internal-ninja files.
set(__bobui_internal_strip_wrapper_programs
    libexec/bobui-internal-strip.in
    libexec/bobui-internal-ninja.in
)
set(__bobui_internal_strip_wrapper_files
    libexec/bobui-internal-strip.bat.in
    libexec/bobui-internal-ninja.bat.in
)
set(__bobui_internal_strip_wrappers
    ${__bobui_internal_strip_wrapper_programs}
    ${__bobui_internal_strip_wrapper_files}
)
bobui_copy_or_install(PROGRAMS
    ${__bobui_internal_strip_wrapper_programs}
    DESTINATION "${__GlobalConfig_install_dir}/libexec"
)
bobui_copy_or_install(FILES
    ${__bobui_internal_strip_wrapper_files}
    DESTINATION "${__GlobalConfig_install_dir}/libexec"
)
if(BOBUI_WILL_INSTALL)
    foreach(__bobui_internal_strip_wrapper ${__bobui_internal_strip_wrappers})
        file(COPY "${__bobui_internal_strip_wrapper}"
             DESTINATION "${__GlobalConfig_build_dir}/libexec")
    endforeach()
endif()

# Wrap previously queried helpers file.
list(TRANSFORM __bobui_cmake_public_helpers PREPEND "cmake/")
list(TRANSFORM __bobui_cmake_public_helpers APPEND ".cmake")

bobui_internal_get_bobui_build_public_files_to_install(__bobui_public_files_to_install)
list(TRANSFORM __bobui_public_files_to_install PREPEND "cmake/")

# Install public CMake files.
# The functions defined inside can be used in both public projects and while building BobUI.
# Usually we put such functions into BobUI6CoreMacros.cmake, but that's getting bloated.
# These files will be included by BobUI6Config.cmake.
set(__public_files
    ${__bobui_cmake_public_helpers}
    ${__bobui_public_files_to_install}
)

bobui_copy_or_install(FILES ${__public_files} DESTINATION "${__GlobalConfig_install_dir}")

# In prefix builds we also need to copy the files into the build config directory, so that the
# build-dir BobUI6Config.cmake finds the files when building examples in-tree.
if(BOBUI_WILL_INSTALL)
    foreach(_public_file ${__public_files})
        file(COPY "${_public_file}" DESTINATION "${__GlobalConfig_build_dir}")
    endforeach()
endif()

bobui_copy_or_install(DIRECTORY "cmake/3rdparty" DESTINATION "${__GlobalConfig_install_dir}")

# In prefix builds we also need to copy the files into the build config directory, so that the
# build-dir BobUI6Config.cmake finds the files when building other repos in a top-level build.
if(BOBUI_WILL_INSTALL)
    file(COPY "cmake/3rdparty" DESTINATION "${__GlobalConfig_build_dir}")
endif()

# Install our custom Find modules, which will be used by the find_dependency() calls
# inside the generated ModuleDependencies cmake files.
bobui_copy_or_install(DIRECTORY cmake/
    DESTINATION "${__GlobalConfig_install_dir}"
    FILES_MATCHING PATTERN "Find*.cmake"
    PATTERN "tests" EXCLUDE
    PATTERN "3rdparty" EXCLUDE
    PATTERN "macos" EXCLUDE
    PATTERN "ios" EXCLUDE
    PATTERN "visionos" EXCLUDE
    PATTERN "platforms" EXCLUDE
    PATTERN "BobUIBuildInternals" EXCLUDE
)

# In prefix builds we also need to copy the files into the build config directory, so that the
# build-dir BobUI6Config.cmake finds the files when building examples as ExternalProjects.
if(BOBUI_WILL_INSTALL)
    file(COPY cmake/
        DESTINATION "${__GlobalConfig_build_dir}"
        FILES_MATCHING PATTERN "Find*.cmake"
        PATTERN "tests" EXCLUDE
        PATTERN "3rdparty" EXCLUDE
        PATTERN "macos" EXCLUDE
        PATTERN "ios" EXCLUDE
        PATTERN "visionos" EXCLUDE
        PATTERN "platforms" EXCLUDE
        PATTERN "BobUIBuildInternals" EXCLUDE
    )
endif()

if(APPLE)
    if(MACOS)
        set(platform_shortname "macos")
    elseif(IOS)
        set(platform_shortname "ios")
    elseif(VISIONOS)
        set(platform_shortname "visionos")
    endif()

    # Info.plist
    bobui_copy_or_install(FILES "cmake/${platform_shortname}/Info.plist.app.in"
        DESTINATION "${__GlobalConfig_install_dir}/${platform_shortname}"
    )
    # For examples built as part of prefix build before install
    file(COPY "cmake/${platform_shortname}/Info.plist.app.in"
        DESTINATION "${__GlobalConfig_build_dir}/${platform_shortname}"
    )

    # Privacy manifest
    bobui_copy_or_install(FILES "cmake/${platform_shortname}/PrivacyInfo.xcprivacy"
        DESTINATION "${__GlobalConfig_install_dir}/${platform_shortname}"
    )
    # For examples built as part of prefix build before install
    file(COPY "cmake/${platform_shortname}/PrivacyInfo.xcprivacy"
        DESTINATION "${__GlobalConfig_build_dir}/${platform_shortname}"
    )

    if(MACOS)
        # Test entitlements
        bobui_copy_or_install(FILES "cmake/${platform_shortname}/test.entitlements.plist"
            DESTINATION "${__GlobalConfig_install_dir}/${platform_shortname}"
        )
        file(COPY "cmake/${platform_shortname}/test.entitlements.plist"
            DESTINATION "${__GlobalConfig_build_dir}/${platform_shortname}"
        )
    elseif(IOS)
        bobui_copy_or_install(FILES "cmake/ios/LaunchScreen.storyboard"
            DESTINATION "${__GlobalConfig_install_dir}/ios"
        )
        # For examples built as part of prefix build before install
        file(COPY "cmake/ios/LaunchScreen.storyboard"
            DESTINATION "${__GlobalConfig_build_dir}/ios"
        )
    endif()
elseif(WASM)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/util/wasm/wasmtestrunner/bobui-wasmtestrunner.py"
        "${BOBUI_BUILD_DIR}/${INSTALL_LIBEXECDIR}/bobui-wasmtestrunner.py" @ONLY)
    bobui_install(PROGRAMS "${BOBUI_BUILD_DIR}/${INSTALL_LIBEXECDIR}/bobui-wasmtestrunner.py"
        DESTINATION "${INSTALL_LIBEXECDIR}")
endif()

# Install CI support files to libexec.
if(BOBUI_INSTALL_CI_FILES)
    bobui_path_join(__bobui_libexec_install_dir "${BOBUI_INSTALL_DIR}" "${INSTALL_LIBEXECDIR}")
    bobui_copy_or_install(FILES coin/instructions/qmake/ensure_pro_file.cmake
        DESTINATION "${__bobui_libexec_install_dir}")
    bobui_copy_or_install(PROGRAMS "util/testrunner/bobui-testrunner.py"
        DESTINATION "${__bobui_libexec_install_dir}")
    bobui_copy_or_install(PROGRAMS "util/testrunner/sanitizer-testrunner.py"
        DESTINATION "${__bobui_libexec_install_dir}")
    bobui_copy_or_install(PROGRAMS "util/json_schema/check_bobui_module_json_schemas.py"
        DESTINATION "${__bobui_libexec_install_dir}")
endif()

# Install json schemas for the users as well
bobui_copy_or_install(FILES
    "util/json_schema/modules.json"
    DESTINATION "${INSTALL_BOBUI_SHAREDIR}/json_schema/")
