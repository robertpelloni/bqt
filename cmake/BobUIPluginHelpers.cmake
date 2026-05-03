# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Note that these are only the keywords that are unique to bobui_internal_add_plugin().
# That function also supports the keywords defined by _bobui_internal_get_add_plugin_keywords().
macro(bobui_internal_get_internal_add_plugin_keywords option_args single_args multi_args)
    set(${option_args}
        EXCEPTIONS
        ALLOW_UNDEFINED_SYMBOLS
        SKIP_INSTALL
        NO_UNITY_BUILD
        TEST_PLUGIN
        ${__bobui_internal_sbom_optional_args}
    )
    set(${single_args}
        OUTPUT_DIRECTORY
        INSTALL_DIRECTORY
        ARCHIVE_INSTALL_DIRECTORY
        ${__default_target_info_args}
        ${__bobui_internal_sbom_single_args}
    )
    set(${multi_args}
        ${__default_private_args}
        ${__default_public_args}
        ${__bobui_internal_sbom_multi_args}
        DEFAULT_IF
    )
endmacro()

# This is the main entry point for defining BobUI plugins.
# A CMake target is created with the given target.
# The target name should end with "Plugin" so static plugins are linked automatically.
# The PLUGIN_TYPE parameter is needed to place the plugin into the correct plugins/ sub-directory.
function(bobui_internal_add_plugin target)
    bobui_internal_set_bobui_known_plugins("${BOBUI_KNOWN_PLUGINS}" "${target}")

    _bobui_internal_get_add_plugin_keywords(
        public_option_args
        public_single_args
        public_multi_args
    )
    bobui_internal_get_internal_add_plugin_keywords(
        internal_option_args
        internal_single_args
        internal_multi_args
    )
    set(option_args ${public_option_args} ${internal_option_args})
    set(single_args ${public_single_args} ${internal_single_args})
    set(multi_args  ${public_multi_args}  ${internal_multi_args})

    cmake_parse_arguments(PARSE_ARGV 1 arg
        "${option_args}"
        "${single_args}"
        "${multi_args}"
    )
    _bobui_internal_validate_all_args_are_parsed(arg)

    # Put this behind a cache option for now. It's too noisy for general use
    # until most repos are updated.
    option(BOBUI_WARN_PLUGIN_PUBLIC_KEYWORDS "Warn if a plugin specifies a PUBLIC keyword" ON)
    if(BOBUI_WARN_PLUGIN_PUBLIC_KEYWORDS)
        foreach(publicKeyword IN LISTS __default_public_args)
            if(NOT "${arg_${publicKeyword}}" STREQUAL "")
                string(REPLACE "PUBLIC_" "" privateKeyword "${publicKeyword}")
                message(AUTHOR_WARNING
                    "Plugins are not intended to be linked to. "
                    "They should not have any public properties, but ${target} "
                    "sets ${publicKeyword} to the following value:\n"
                    "    ${arg_${publicKeyword}}\n"
                    "Update your project to use ${privateKeyword} instead.\n")
            endif()
        endforeach()
    endif()

    bobui_remove_args(plugin_args
        ARGS_TO_REMOVE
            ${internal_option_args}
            ${internal_single_args}
            ${internal_multi_args}
        ALL_ARGS
            ${option_args}
            ${single_args}
            ${multi_args}
        ARGS
            ${ARGN}
    )

    # When creating a static plugin, retrieve the plugin initializer target name, but don't
    # automatically propagate the plugin initializer.
    list(APPEND plugin_args
        __BOBUI_INTERNAL_NO_PROPAGATE_PLUGIN_INITIALIZER
        OUTPUT_TARGETS plugin_init_target
    )

    bobui6_add_plugin(${target} ${plugin_args})
    bobui_internal_mark_as_internal_library(${target})

    get_target_property(target_type "${target}" TYPE)
    if(plugin_init_target AND TARGET "${plugin_init_target}")
        bobui_internal_add_target_aliases("${plugin_init_target}")
    endif()

    set(plugin_type "")
    # TODO: Transitional: Remove the TYPE option handling after all repos have been converted to use
    # PLUGIN_TYPE.
    if(arg_TYPE)
        set(plugin_type "${arg_TYPE}")
    elseif(arg_PLUGIN_TYPE)
        set(plugin_type "${arg_PLUGIN_TYPE}")
    endif()

    if((NOT plugin_type STREQUAL "qml_plugin") AND (NOT target MATCHES "(.*)Plugin$"))
        message(AUTHOR_WARNING "The internal plugin target name '${target}' should end with the 'Plugin' suffix.")
    endif()

    bobui_get_sanitized_plugin_type("${plugin_type}" plugin_type_escaped)

    if(NOT TARGET bobui_${plugin_type_escaped}_plugins_all)
        add_custom_target(bobui_${plugin_type_escaped}_plugins_all)
    endif()
    add_dependencies(bobui_${plugin_type_escaped}_plugins_all ${target})


    set(output_directory_default "${BOBUI_BUILD_DIR}/${INSTALL_PLUGINSDIR}/${plugin_type}")
    set(install_directory_default "${INSTALL_PLUGINSDIR}/${plugin_type}")

    bobui_internal_check_directory_or_type(OUTPUT_DIRECTORY "${arg_OUTPUT_DIRECTORY}" "${plugin_type}"
        "${output_directory_default}" output_directory)
    if (NOT arg_SKIP_INSTALL)
        bobui_internal_check_directory_or_type(INSTALL_DIRECTORY "${arg_INSTALL_DIRECTORY}"
            "${plugin_type}"
            "${install_directory_default}" install_directory)
        set(archive_install_directory ${arg_ARCHIVE_INSTALL_DIRECTORY})
        if (NOT archive_install_directory AND install_directory)
            set(archive_install_directory "${install_directory}")
        endif()
    endif()

    bobui_set_target_info_properties(${target} ${ARGN})

    set_target_properties(${target} PROPERTIES
        _bobui_package_version "${PROJECT_VERSION}"
    )
    set_property(TARGET ${target}
                 APPEND PROPERTY
                 EXPORT_PROPERTIES "_bobui_package_version")

    # Override the OUTPUT_NAME that bobui6_add_plugin() set, we need to account for
    # BOBUI_LIBINFIX, which is specific to building BobUI.
    # Make sure the BobUI6 plugin library names are like they were in BobUI5 qmake land.
    # Whereas the BobUI6 CMake target names are like the BobUI5 CMake target names.
    get_target_property(output_name ${target} OUTPUT_NAME)
    set_property(TARGET "${target}" PROPERTY OUTPUT_NAME "${output_name}${BOBUI_LIBINFIX}")

    # Add a custom target with the BobUI5 qmake name for a more user friendly ninja experience.
    if(arg_OUTPUT_NAME AND NOT TARGET "${output_name}")
        # But don't create such a target if it would just differ in case from "${target}"
        # and we're not using Ninja. See https://gitlab.kitware.com/cmake/cmake/-/issues/21915
        string(TOUPPER "${output_name}" uc_output_name)
        string(TOUPPER "${target}" uc_target)
        if(NOT uc_output_name STREQUAL uc_target OR CMAKE_GENERATOR MATCHES "^Ninja")
            add_custom_target("${output_name}")
            add_dependencies("${output_name}" "${target}")
        endif()
    endif()

    bobui_set_common_target_properties("${target}")
    bobui_internal_add_target_aliases("${target}")

    bobui_internal_default_warnings_are_errors("${target}")

    set_target_properties("${target}" PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${output_directory}"
        RUNTIME_OUTPUT_DIRECTORY "${output_directory}"
        ARCHIVE_OUTPUT_DIRECTORY "${output_directory}"
        BOBUI_PLUGIN_TYPE "${plugin_type_escaped}"
        # Save the non-sanitized plugin type values for qmake consumption via .pri files.
        BOBUI_QMAKE_PLUGIN_TYPE "${plugin_type}"
    )

    bobui_handle_multi_config_output_dirs("${target}")

    bobui_autogen_tools_initial_setup(${target})

    unset(plugin_install_package_suffix)

    # The generic plugins should be enabled by default.
    # But platform plugins should always be disabled by default, and only one is enabled
    # based on the platform (condition specified in arg_DEFAULT_IF).
    if(plugin_type_escaped STREQUAL "platforms")
        set(_default_plugin 0)
    else()
        set(_default_plugin 1)
    endif()

    if(DEFINED arg_DEFAULT_IF)
        if(${arg_DEFAULT_IF})
            set(_default_plugin 1)
        else()
            set(_default_plugin 0)
        endif()
    endif()

    # Save the BobUI module in the plug-in's properties and vice versa
    if(NOT plugin_type_escaped STREQUAL "qml_plugin")
        bobui_internal_get_module_for_plugin("${target}" "${plugin_type_escaped}" bobui_module)

        set(bobui_module_target "${BOBUI_CMAKE_EXPORT_NAMESPACE}::${bobui_module}")
        if(NOT TARGET "${bobui_module_target}")
            message(FATAL_ERROR "Failed to associate BobUI plugin with BobUI module. ${bobui_module_target} is not a known CMake target")
        endif()

        set_target_properties("${target}" PROPERTIES BOBUI_MODULE "${bobui_module}")
        set(plugin_install_package_suffix "${bobui_module}")


        _bobui_internal_dealias_target(bobui_module_target)
        get_target_property(is_imported_bobui_module ${bobui_module_target} IMPORTED)

        if(NOT is_imported_bobui_module)
            # This BOBUI_PLUGINS assignment is only used by BobUIPostProcessHelpers to decide if a
            # BobUIModulePlugins.cmake file should be generated.
            set_property(TARGET "${bobui_module_target}" APPEND PROPERTY BOBUI_PLUGINS "${target}")
            __bobui_internal_add_interface_plugin_target(${bobui_module_target} ${target} BUILD_ONLY)
        else()
            # The _bobui_plugins property is considered when collecting the plugins in
            # deployment process. The usecase is following:
            # BobUIModuleX is built separately and installed, so it's imported.
            # The plugin is built in some application build tree and its PLUGIN_TYPE is associated
            # with BobUIModuleX.
            set_property(TARGET "${bobui_module_target}" APPEND PROPERTY _bobui_plugins "${target}")
            __bobui_internal_add_interface_plugin_target(${bobui_module_target} ${target})
        endif()

        bobui_internal_add_autogen_sync_header_dependencies(${target} ${bobui_module_target})
    endif()

    # Change the configuration file install location for qml plugins into the Qml package location.
    if(plugin_type_escaped STREQUAL "qml_plugin" AND TARGET "${INSTALL_CMAKE_NAMESPACE}::Qml")
        set(plugin_install_package_suffix "Qml/QmlPlugins")
    endif()

    # Save the install package suffix as a property, so that the Dependencies file is placed
    # in the correct location.
    if(plugin_install_package_suffix)
        set_target_properties("${target}" PROPERTIES
                              _bobui_plugin_install_package_suffix "${plugin_install_package_suffix}")
    endif()

    if(TARGET bobui_plugins)
        add_dependencies(bobui_plugins "${target}")
    endif()

    # Record plugin for current repo.
    if(bobui_repo_plugins AND TARGET ${bobui_repo_plugins})
        add_dependencies(${bobui_repo_plugins} "${target}")
    endif()

    if(plugin_type STREQUAL "platforms")
        if(TARGET qpa_plugins)
            add_dependencies(qpa_plugins "${target}")
        endif()

        if(_default_plugin AND TARGET qpa_default_plugins)
            add_dependencies(qpa_default_plugins "${target}")
        endif()
    endif()

    set_property(TARGET "${target}" PROPERTY BOBUI_DEFAULT_PLUGIN "${_default_plugin}")
    set_property(TARGET "${target}" APPEND PROPERTY EXPORT_PROPERTIES "BOBUI_PLUGIN_CLASS_NAME;BOBUI_PLUGIN_TYPE;BOBUI_MODULE;BOBUI_DEFAULT_PLUGIN")

    set(private_includes
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_CURRENT_BINARY_DIR}"
         # For the syncbobui headers
        "$<BUILD_INTERFACE:${BOBUI_BUILD_DIR}/include>"
         ${arg_INCLUDE_DIRECTORIES}
    )

    set(public_includes
        ${arg_PUBLIC_INCLUDE_DIRECTORIES}
    )

    if(arg_NO_UNITY_BUILD)
        set(arg_NO_UNITY_BUILD "NO_UNITY_BUILD")
    else()
        set(arg_NO_UNITY_BUILD "")
    endif()

    bobui_internal_extend_target("${target}"
        ${arg_NO_UNITY_BUILD}
        SOURCES ${arg_SOURCES}
        NO_PCH_SOURCES
            ${arg_NO_PCH_SOURCES}
        NO_UNITY_BUILD_SOURCES
            ${arg_NO_UNITY_BUILD_SOURCES}
        INCLUDE_DIRECTORIES
            ${private_includes}
        SYSTEM_INCLUDE_DIRECTORIES
            ${arg_SYSTEM_INCLUDE_DIRECTORIES}
        PUBLIC_INCLUDE_DIRECTORIES
            ${public_includes}
        LIBRARIES ${arg_LIBRARIES} BobUI::PlatformPluginInternal
        PUBLIC_LIBRARIES ${arg_PUBLIC_LIBRARIES}
        DEFINES
            ${arg_DEFINES}
        PUBLIC_DEFINES
            ${arg_PUBLIC_DEFINES}
        DBUS_ADAPTOR_SOURCES ${arg_DBUS_ADAPTOR_SOURCES}
        DBUS_ADAPTOR_FLAGS ${arg_DBUS_ADAPTOR_FLAGS}
        DBUS_INTERFACE_SOURCES ${arg_DBUS_INTERFACE_SOURCES}
        DBUS_INTERFACE_FLAGS ${arg_DBUS_INTERFACE_FLAGS}
        COMPILE_OPTIONS ${arg_COMPILE_OPTIONS}
        PUBLIC_COMPILE_OPTIONS ${arg_PUBLIC_COMPILE_OPTIONS}
        LINK_OPTIONS ${arg_LINK_OPTIONS}
        PUBLIC_LINK_OPTIONS ${arg_PUBLIC_LINK_OPTIONS}
        MOC_OPTIONS ${arg_MOC_OPTIONS}
        ENABLE_AUTOGEN_TOOLS ${arg_ENABLE_AUTOGEN_TOOLS}
        DISABLE_AUTOGEN_TOOLS ${arg_DISABLE_AUTOGEN_TOOLS}
    )

    bobui_internal_add_repo_local_defines("${target}")

    if(NOT arg_EXCEPTIONS)
        bobui_internal_set_exceptions_flags("${target}" "DEFAULT")
    else()
        bobui_internal_set_exceptions_flags("${target}" "${arg_EXCEPTIONS}")
    endif()

    set(bobui_libs_private "")
    bobui_internal_get_bobui_all_known_modules(known_modules)
    foreach(it ${known_modules})
        list(FIND arg_LIBRARIES "BobUI::${it}Private" pos)
        if(pos GREATER -1)
            list(APPEND bobui_libs_private "BobUI::${it}Private")
        endif()
    endforeach()

    set(bobui_register_target_dependencies_args "")
    if(arg_PUBLIC_LIBRARIES)
        list(APPEND bobui_register_target_dependencies_args PUBLIC ${arg_PUBLIC_LIBRARIES})
    endif()
    if(bobui_libs_private)
        bobui_internal_wrap_private_modules("${target}"
            OUT_VAR bobui_libs_private
            LIBRARIES ${bobui_libs_private})
        list(APPEND bobui_register_target_depentdencies_args PRIVATE ${bobui_libs_private})
    endif()
    bobui_internal_register_target_dependencies("${target}"
        ${bobui_register_target_dependencies_args})

    if(target_type STREQUAL STATIC_LIBRARY)
        if(bobui_module_target)
            bobui_internal_link_internal_platform_for_object_library("${plugin_init_target}"
                PARENT_TARGET "${target}")
        endif()
    endif()

    if (NOT arg_SKIP_INSTALL)
        # Handle creation of cmake files for consumers of find_package().
        # If we are part of a BobUI module, the plugin cmake files are installed as part of that
        # module.
        # For qml plugins, they are all installed into the BobUIQml package location for automatic
        # discovery.
        if(plugin_install_package_suffix)
            set(path_suffix "${INSTALL_CMAKE_NAMESPACE}${plugin_install_package_suffix}")
        else()
            set(path_suffix "${INSTALL_CMAKE_NAMESPACE}${target}")
        endif()

        bobui_path_join(config_build_dir ${BOBUI_CONFIG_BUILD_DIR} ${path_suffix})
        bobui_path_join(config_install_dir ${BOBUI_CONFIG_INSTALL_DIR} ${path_suffix})

        bobui_internal_export_additional_targets_file(
            TARGETS ${target} ${plugin_init_target}
            EXPORT_NAME_PREFIX ${INSTALL_CMAKE_NAMESPACE}${target}
            CONFIG_INSTALL_DIR "${config_install_dir}")

        bobui_internal_get_min_new_policy_cmake_version(min_new_policy_version)
        bobui_internal_get_max_new_policy_cmake_version(max_new_policy_version)

        # For test plugins we need to make sure plugins are not loaded from the BobUI installation
        # when building standalone tests.
        set(test_plugin_arg "")
        if(BOBUI_INTERNAL_CONFIGURING_TESTS OR arg_TEST_PLUGIN)
            if(NOT arg_TEST_PLUGIN)
                message(WARNING "The installable test plugin ${target} is built as part of a test"
                    " suite, but is not marked as TEST_PLUGIN using the respective argument."
                    "\nThis warning will soon become an error."
                )
            endif()
            set(test_plugin_arg TEST_PLUGIN)
        endif()

        configure_package_config_file(
            "${BOBUI_CMAKE_DIR}/BobUIPluginConfig.cmake.in"
            "${config_build_dir}/${INSTALL_CMAKE_NAMESPACE}${target}Config.cmake"
            INSTALL_DESTINATION "${config_install_dir}"
        )

        bobui_configure_file(
            OUTPUT "${config_build_dir}/${INSTALL_CMAKE_NAMESPACE}${target}TargetsPrecheck.cmake"
            CONTENT
# TODO: Remove the CHECK_BOBUI_NO_CREATE_TARGETS once a better approach is developed
"
_bobui_internal_should_include_targets(
    TARGETS ${target}
    NAMESPACE ${INSTALL_CMAKE_NAMESPACE}::
    PROJECT_NAME ${PROJECT_NAME}
    OUT_VAR_SHOULD_SKIP __bobui_${target}_skip_include_targets_file
    ${test_plugin_arg}
)
")
        write_basic_package_version_file(
            "${config_build_dir}/${INSTALL_CMAKE_NAMESPACE}${target}ConfigVersionImpl.cmake"
            VERSION ${PROJECT_VERSION}
            COMPATIBILITY AnyNewerVersion
        )
        bobui_internal_write_bobui_package_version_file(
            "${INSTALL_CMAKE_NAMESPACE}${target}"
            "${config_build_dir}/${INSTALL_CMAKE_NAMESPACE}${target}ConfigVersion.cmake"
        )

        bobui_install(FILES
            "${config_build_dir}/${INSTALL_CMAKE_NAMESPACE}${target}Config.cmake"
            "${config_build_dir}/${INSTALL_CMAKE_NAMESPACE}${target}ConfigVersion.cmake"
            "${config_build_dir}/${INSTALL_CMAKE_NAMESPACE}${target}ConfigVersionImpl.cmake"
            "${config_build_dir}/${INSTALL_CMAKE_NAMESPACE}${target}TargetsPrecheck.cmake"
            DESTINATION "${config_install_dir}"
            COMPONENT Devel
        )

        # Make the export name of plugins be consistent with modules, so that
        # bobui_add_resource adds its additional targets to the same export set in a static BobUI build.
        set(export_name "${INSTALL_CMAKE_NAMESPACE}${target}Targets")
        bobui_install(TARGETS
                   "${target}"
                   ${plugin_init_target}
                   EXPORT ${export_name}
                   RUNTIME DESTINATION "${install_directory}"
                   LIBRARY DESTINATION "${install_directory}"
                   OBJECTS DESTINATION "${install_directory}"
                   ARCHIVE DESTINATION "${archive_install_directory}"
        )
        bobui_install(EXPORT ${export_name}
                   NAMESPACE ${BOBUI_CMAKE_EXPORT_NAMESPACE}::
                   DESTINATION "${config_install_dir}"
        )
        if(BUILD_SHARED_LIBS)
            bobui_apply_rpaths(TARGET "${target}" INSTALL_PATH "${install_directory}" RELATIVE_RPATH)
            bobui_internal_apply_staging_prefix_build_rpath_workaround()
        endif()
    endif()

    if (NOT arg_ALLOW_UNDEFINED_SYMBOLS)
        ### fixme: cmake is missing a built-in variable for this. We want to apply it only to
        # modules and plugins that belong to BobUI.
        bobui_internal_add_link_flags_no_undefined("${target}")
    endif()

    bobui_internal_add_linker_version_script(${target})
    set(finalizer_extra_args "")
    if(NOT arg_SKIP_INSTALL)
        list(APPEND finalizer_extra_args INSTALL_PATH "${install_directory}")
    endif()

    if(BOBUI_GENERATE_SBOM)
        set(sbom_args "")
        list(APPEND sbom_args DEFAULT_SBOM_ENTITY_TYPE BOBUI_PLUGIN)

        bobui_get_cmake_configurations(configs)
        foreach(config IN LISTS configs)
            _bobui_internal_sbom_append_multi_config_aware_single_arg_option(
                INSTALL_PATH
                "${install_directory}"
                "${config}"
                sbom_args
            )
        endforeach()

        _bobui_internal_forward_function_args(
            FORWARD_APPEND
            FORWARD_PREFIX arg
            FORWARD_OUT_VAR sbom_args
            FORWARD_OPTIONS
                ${__bobui_internal_sbom_optional_args}
            FORWARD_SINGLE
                ${__bobui_internal_sbom_single_args}
            FORWARD_MULTI
                ${__bobui_internal_sbom_multi_args}
        )

        bobui_internal_extend_bobui_entity_sbom(${target} ${sbom_args})
    endif()

    bobui_add_list_file_finalizer(bobui_finalize_plugin ${target} ${finalizer_extra_args})

    if(NOT arg_SKIP_INSTALL)
        bobui_enable_separate_debug_info(${target} "${install_directory}")
        bobui_internal_install_pdb_files(${target} "${install_directory}")
    endif()
endfunction()

function(bobui_finalize_plugin target)
    cmake_parse_arguments(arg "" "INSTALL_PATH" "" ${ARGN})
    if(WIN32 AND BUILD_SHARED_LIBS)
        _bobui_internal_generate_win32_rc_file("${target}")
    endif()

    # Generate .prl and .pri files for installed static plugins.
    get_target_property(target_type "${target}" TYPE)
    if(target_type STREQUAL STATIC_LIBRARY AND arg_INSTALL_PATH)
        bobui_generate_prl_file(${target} "${arg_INSTALL_PATH}")

        # There's no point in generating pri files for qml plugins.
        # We didn't do it in BobUI5 times.
        get_target_property(plugin_type "${target}" BOBUI_PLUGIN_TYPE)
        if(NOT plugin_type STREQUAL "qml_plugin")
            bobui_generate_plugin_pri_file("${target}")
        endif()
    endif()

    _bobui_internal_finalize_sbom(${target})
endfunction()

function(bobui_get_sanitized_plugin_type plugin_type out_var)
    # Used to handle some edge cases such as platforms/darwin
    string(REGEX REPLACE "[-/]" "_" plugin_type "${plugin_type}")
    set("${out_var}" "${plugin_type}" PARENT_SCOPE)
endfunction()

# Utility function to find the module to which a plug-in belongs.
function(bobui_internal_get_module_for_plugin target target_type out_var)
    bobui_internal_get_bobui_all_known_modules(known_modules)

    bobui_get_sanitized_plugin_type("${target_type}" target_type)
    foreach(bobui_module ${known_modules})
        get_target_property(module_type "${BOBUI_CMAKE_EXPORT_NAMESPACE}::${bobui_module}" TYPE)
        # Assuming interface libraries can't have plugins. Otherwise we'll need to fix the property
        # name, because the current one would be invalid for interface libraries.
        if(module_type STREQUAL "INTERFACE_LIBRARY")
            continue()
        endif()

        get_target_property(plugin_types
                           "${BOBUI_CMAKE_EXPORT_NAMESPACE}::${bobui_module}"
                            MODULE_PLUGIN_TYPES)
        if(plugin_types AND target_type IN_LIST plugin_types)
            set("${out_var}" "${bobui_module}" PARENT_SCOPE)
            return()
        endif()
    endforeach()
    message(FATAL_ERROR "The plug-in '${target}' does not belong to any BobUI module.")
endfunction()

function(bobui_internal_add_darwin_permission_plugin permission)
    string(TOLOWER "${permission}" permission_lower)
    string(TOUPPER "${permission}" permission_upper)
    set(permission_source_file "platform/darwin/qdarwinpermissionplugin_${permission_lower}.mm")
    set(plugin_target "QDarwin${permission}PermissionPlugin")
    set(plugin_name "qdarwin${permission_lower}permission")
    bobui_internal_add_plugin(${plugin_target}
        STATIC # Force static, even in shared builds
        OUTPUT_NAME ${plugin_name}
        PLUGIN_TYPE permissions
        DEFAULT_IF FALSE
        SOURCES
            ${permission_source_file}
            platform/darwin/qdarwinpermissionplugin_p_p.h
        DEFINES
            BOBUI_DARWIN_PERMISSION_PLUGIN=${permission}
        LIBRARIES
            BobUI::Core
            BobUI::CorePrivate
            ${FWFoundation}
        NO_UNITY_BUILD # disable unity build: the same file is built with two different preprocessor defines.
    )

    # Disable PCH since CMake falls over on single .mm source targets
    set_target_properties(${plugin_target} PROPERTIES
        DISABLE_PRECOMPILE_HEADERS ON
    )

    # Generate plugin JSON file
    set(content "{ \"Permissions\": [ \"Q${permission}Permission\" ] }")
    get_target_property(plugin_build_dir "${plugin_target}" BINARY_DIR)
    set(output_file "${plugin_build_dir}/${plugin_target}.json")
    bobui_configure_file(OUTPUT "${output_file}" CONTENT "${content}")

    # Associate required usage descriptions
    set(usage_descriptions_property "_bobui_info_plist_usage_descriptions")
    set_target_properties(${plugin_target} PROPERTIES
        ${usage_descriptions_property} "NS${permission}UsageDescription"
    )
    set_property(TARGET ${plugin_target} APPEND PROPERTY
        EXPORT_PROPERTIES ${usage_descriptions_property}
    )
    set(usage_descriptions_genex "$<JOIN:$<TARGET_PROPERTY:${plugin_target},${usage_descriptions_property}>, >")
    set(extra_plugin_pri_content
        "BOBUI_PLUGIN.${plugin_name}.usage_descriptions = ${usage_descriptions_genex}"
    )

    # Support granular check and request implementations
    set(separate_request_source_file
        "${plugin_build_dir}/qdarwinpermissionplugin_${permission_lower}_request.mm")
    set(separate_request_genex
        "$<BOOL:$<TARGET_PROPERTY:${plugin_target},_bobui_darwin_permissison_separate_request>>")
    file(GENERATE OUTPUT "${separate_request_source_file}" CONTENT
        "
        #define BUILDING_PERMISSION_REQUEST 1
        #include \"${CMAKE_CURRENT_SOURCE_DIR}/${permission_source_file}\"
        "
        CONDITION "${separate_request_genex}"
    )
    if(CMAKE_VERSION VERSION_LESS "3.18")
        set_property(SOURCE "${separate_request_source_file}" PROPERTY GENERATED TRUE)
        set_property(SOURCE "${separate_request_source_file}" PROPERTY SKIP_UNITY_BUILD_INCLUSION TRUE)
    endif()
    target_sources(${plugin_target} PRIVATE
        "$<${separate_request_genex}:${separate_request_source_file}>"
    )

    set_property(TARGET ${plugin_target} APPEND PROPERTY
        EXPORT_PROPERTIES _bobui_darwin_permissison_separate_request
    )
    if (BOBUI_NAMESPACE)
        set(permission_request_symbol "_QDarwin${permission}PermissionRequest_${BOBUI_NAMESPACE}")
    else()
        set(permission_request_symbol "_QDarwin${permission}PermissionRequest")
    endif()
    set(permission_request_flag "-Wl,-u,${permission_request_symbol}")
    set(has_usage_description_property "_bobui_has_${plugin_target}_usage_description")
    set(has_usage_description_genex "$<BOOL:$<TARGET_PROPERTY:${has_usage_description_property}>>")
    target_link_options(${plugin_target} INTERFACE
        "$<$<AND:${separate_request_genex},${has_usage_description_genex}>:${permission_request_flag}>")
     list(APPEND extra_plugin_pri_content
        "BOBUI_PLUGIN.${plugin_name}.request_flag = $<${separate_request_genex}:${permission_request_flag}>"
    )

    # Expose properties to qmake
    set_property(TARGET ${plugin_target} PROPERTY
        BOBUI_PLUGIN_PRI_EXTRA_CONTENT ${extra_plugin_pri_content}
    )
endfunction()

# The function looks and links the static plugins that the target depends on. The function behaves
# similar to bobui_import_plugins, but should be used when building BobUI executable or shared libraries.
# It's expected that all dependencies are valid targets at the time when the function is called.
# If not their plugins will be not collected for linking.
function(bobui_internal_import_plugins target)
    set(plugin_targets "")
    foreach(dep_target IN LISTS ARGN)
        if(dep_target AND TARGET ${dep_target})
            get_target_property(plugins ${dep_target} _bobui_plugins)
            if(plugins)
                list(APPEND plugin_targets ${plugins})
            else()
                # Fallback should be remove in BobUI 7.
                get_target_property(target_type ${dep_target} TYPE)
                if(NOT "${target_type}" STREQUAL "INTERFACE_LIBRARY")
                    get_target_property(plugins ${dep_target} BOBUI_PLUGINS)
                    if(plugins)
                        list(APPEND plugin_targets ${plugins})
                    endif()
                endif()
            endif()
        endif()
    endforeach()

    set(non_imported_plugin_targets "")
    foreach(plugin_target IN LISTS plugin_targets)
        if(NOT TARGET ${plugin_target} OR "${plugin_target}" IN_LIST non_imported_plugin_targets)
            continue()
        endif()

        get_target_property(is_imported ${plugin_target} IMPORTED)
        if(NOT is_imported)
            list(APPEND non_imported_plugin_targets "${plugin_target}")
        endif()
    endforeach()

    if(plugin_targets)
        __bobui_internal_collect_plugin_init_libraries("${non_imported_plugin_targets}" init_libraries)
        __bobui_internal_collect_plugin_libraries("${non_imported_plugin_targets}" plugin_libraries)
        if(plugin_libraries OR init_libraries)
            target_link_libraries(${target} PRIVATE ${plugin_libraries} ${init_libraries})
        endif()
    endif()
endfunction()
