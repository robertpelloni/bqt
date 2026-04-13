macro(bobui_internal_get_add_library_option_args option_args)
    set(${option_args}
        SHARED
        STATIC
        MODULE
        INTERFACE
        NO_UNITY_BUILD
        )
endmacro()

# Helper to create a library using the public _bobui_internal_add_library function.
#
# The difference to _bobui_internal_add_library is that MODULE is replaced with STATIC in a static
# BobUI build.
# Everything else is just prepation for option validating.
function(bobui_internal_add_common_bobui_library_helper target)
    bobui_internal_get_add_library_option_args(option_args)
    cmake_parse_arguments(PARSE_ARGV 1 arg
        "${option_args}"
        ""
        ""
    )
    _bobui_internal_validate_all_args_are_parsed(arg)

    if(arg_SHARED)
        set(arg_SHARED SHARED)
    else()
        set(arg_SHARED "")
    endif()

    if(arg_MODULE)
        set(arg_MODULE MODULE)
    else()
        set(arg_MODULE "")
    endif()

    if(arg_STATIC)
        set(arg_STATIC STATIC)
    else()
        set(arg_STATIC "")
    endif()

    if(arg_INTERFACE)
        set(arg_INTERFACE INTERFACE)
    else()
        set(arg_INTERFACE "")
    endif()

    if(arg_MODULE AND NOT BUILD_SHARED_LIBS)
        set(arg_MODULE STATIC)
    endif()

    _bobui_internal_add_library(${target} ${arg_STATIC} ${arg_SHARED} ${arg_MODULE} ${arg_INTERFACE})

    if(arg_NO_UNITY_BUILD)
        set_property(TARGET "${target}" PROPERTY UNITY_BUILD OFF)
    endif()

    bobui_internal_mark_as_internal_library(${target})
endfunction()

# Wrapper function to create a regular cmake target and forward all the
# arguments collected by the conversion script.
function(bobui_internal_add_cmake_library target)
    bobui_internal_get_add_library_option_args(option_args)
    set(single_args
        OUTPUT_DIRECTORY
        ARCHIVE_INSTALL_DIRECTORY
        INSTALL_DIRECTORY
    )
    set(multi_args
        ${__default_private_args}
        ${__default_public_args}
    )

    cmake_parse_arguments(PARSE_ARGV 1 arg
        "${option_args}"
        "${single_args}"
        "${multi_args}"
    )
    _bobui_internal_validate_all_args_are_parsed(arg)
    _bobui_internal_validate_no_unity_build(arg)

    bobui_remove_args(library_helper_args
        ARGS_TO_REMOVE
            ${single_args}
            ${multi_args}
        ALL_ARGS
            ${option_args}
            ${single_args}
            ${multi_args}
        ARGS
            ${ARGN}
    )

    bobui_internal_add_common_bobui_library_helper(${target} ${library_helper_args})

    bobui_internal_default_warnings_are_errors("${target}")

    if (arg_OUTPUT_DIRECTORY)
        set_target_properties(${target} PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY ${arg_OUTPUT_DIRECTORY}
            RUNTIME_OUTPUT_DIRECTORY ${arg_OUTPUT_DIRECTORY}
            LIBRARY_OUTPUT_DIRECTORY ${arg_OUTPUT_DIRECTORY}
        )
    endif()

    _bobui_internal_forward_function_args(
        FORWARD_PREFIX arg
        FORWARD_OUT_VAR extend_target_args
        FORWARD_MULTI
            SOURCES
            NO_PCH_SOURCES
            INCLUDE_DIRECTORIES
            SYSTEM_INCLUDE_DIRECTORIES
            PUBLIC_INCLUDE_DIRECTORIES
            PUBLIC_DEFINES
            DEFINES
            PUBLIC_LIBRARIES
            COMPILE_OPTIONS
            PUBLIC_COMPILE_OPTIONS
            LINK_OPTIONS
            PUBLIC_LINK_OPTIONS
            MOC_OPTIONS
            ENABLE_AUTOGEN_TOOLS
            DISABLE_AUTOGEN_TOOLS
    )

    bobui_internal_extend_target("${target}"
        ${extend_target_args}
        LIBRARIES ${arg_LIBRARIES} BobUI::PlatformCommonInternal
        NO_UNITY_BUILD # Disabled by default
    )
endfunction()

# This function replaces qmake's bobui_helper_lib feature. It is intended to
# compile 3rdparty libraries as part of the build.
#
function(bobui_internal_add_3rdparty_library target)
    bobui_internal_get_add_library_option_args(library_option_args)

    set(option_args
        EXCEPTIONS
        INSTALL
        SKIP_AUTOMOC
        ${__bobui_internal_sbom_optional_args}
    )
    set(single_args
        OUTPUT_DIRECTORY
        QMAKE_LIB_NAME
        ${__bobui_internal_sbom_single_args}
    )
    set(multi_args
        ${__default_private_args}
        ${__default_public_args}
        ${__bobui_internal_sbom_multi_args}
    )

    cmake_parse_arguments(PARSE_ARGV 1 arg
        "${library_option_args};${option_args}"
        "${single_args}"
        "${multi_args}"
    )
    _bobui_internal_validate_all_args_are_parsed(arg)
    _bobui_internal_validate_no_unity_build(arg)

    bobui_remove_args(library_helper_args
        ARGS_TO_REMOVE
            ${option_args}
            ${single_args}
            ${multi_args}
        ALL_ARGS
            ${library_option_args}
            ${option_args}
            ${single_args}
            ${multi_args}
        ARGS
            ${ARGN}
    )

    bobui_internal_add_common_bobui_library_helper(${target} ${library_helper_args})

    set_target_properties(${target} PROPERTIES
        _bobui_module_interface_name "${target}"
        _bobui_package_version "${PROJECT_VERSION}"
        _bobui_package_name "${INSTALL_CMAKE_NAMESPACE}${target}"
        _bobui_module_is_3rdparty_library TRUE
    )

    set(export_properties
        "_bobui_module_interface_name"
        "_bobui_package_version"
        "_bobui_package_name"
        "_bobui_module_is_3rdparty_library"
    )

    set_property(TARGET ${target}
                 APPEND PROPERTY
                 EXPORT_PROPERTIES "${export_properties}")

    bobui_internal_add_bobui_repo_known_module(${target})
    bobui_internal_add_target_aliases(${target})

    bobui_internal_default_warnings_are_errors("${target}")

    set_target_properties(${target} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${BOBUI_BUILD_DIR}/${INSTALL_LIBDIR}"
        RUNTIME_OUTPUT_DIRECTORY "${BOBUI_BUILD_DIR}/${INSTALL_BINDIR}"
        ARCHIVE_OUTPUT_DIRECTORY "${BOBUI_BUILD_DIR}/${INSTALL_LIBDIR}"
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        _bobui_module_skip_depends_include TRUE
    )
    set_property(TARGET "${target}"
                 APPEND PROPERTY EXPORT_PROPERTIES _bobui_module_is_3rdparty_library)
    set_property(TARGET "${target}"
                 APPEND PROPERTY EXPORT_PROPERTIES _bobui_module_skip_depends_include)

    bobui_handle_multi_config_output_dirs("${target}")

    set_target_properties(${target} PROPERTIES
        OUTPUT_NAME "${INSTALL_CMAKE_NAMESPACE}${target}"
    )

    if(NOT arg_INTERFACE)
        bobui_set_common_target_properties(${target})
    endif()

    if(NOT arg_SKIP_AUTOMOC)
        bobui_autogen_tools_initial_setup(${target})
    endif()

    if(NOT arg_EXCEPTIONS)
        bobui_internal_set_exceptions_flags("${target}" "DEFAULT")
    else()
        bobui_internal_set_exceptions_flags("${target}" "${arg_EXCEPTIONS}")
    endif()

    bobui_internal_extend_target("${target}"
        SOURCES ${arg_SOURCES}
        INCLUDE_DIRECTORIES
            ${arg_INCLUDE_DIRECTORIES}
        PUBLIC_INCLUDE_DIRECTORIES
            ${arg_PUBLIC_INCLUDE_DIRECTORIES}
        PUBLIC_DEFINES
            ${arg_PUBLIC_DEFINES}
        DEFINES
            ${arg_DEFINES}
        PUBLIC_LIBRARIES ${arg_PUBLIC_LIBRARIES}
        LIBRARIES ${arg_LIBRARIES} BobUI::PlatformModuleInternal
        COMPILE_OPTIONS ${arg_COMPILE_OPTIONS}
        PUBLIC_COMPILE_OPTIONS ${arg_PUBLIC_COMPILE_OPTIONS}
        LINK_OPTIONS ${arg_LINK_OPTIONS}
        PUBLIC_LINK_OPTIONS ${arg_PUBLIC_LINK_OPTIONS}
        MOC_OPTIONS ${arg_MOC_OPTIONS}
        ENABLE_AUTOGEN_TOOLS ${arg_ENABLE_AUTOGEN_TOOLS}
        DISABLE_AUTOGEN_TOOLS ${arg_DISABLE_AUTOGEN_TOOLS}
        NO_UNITY_BUILD
    )

    if(NOT BUILD_SHARED_LIBS OR arg_INSTALL)
        set(will_install TRUE)
    else()
        set(will_install FALSE)
    endif()
    set_target_properties("${target}" PROPERTIES _bobui_will_install ${will_install})

    if(will_install)
        bobui_generate_3rdparty_lib_pri_file("${target}" "${arg_QMAKE_LIB_NAME}" pri_file)
        if(pri_file)
            bobui_install(FILES "${pri_file}" DESTINATION "${INSTALL_MKSPECSDIR}/modules")
        endif()

        set(path_suffix "${INSTALL_CMAKE_NAMESPACE}${target}")
        bobui_path_join(config_build_dir ${BOBUI_CONFIG_BUILD_DIR} ${path_suffix})
        bobui_path_join(config_install_dir ${BOBUI_CONFIG_INSTALL_DIR} ${path_suffix})
        set(export_name "${INSTALL_CMAKE_NAMESPACE}${target}Targets")

        bobui_internal_get_min_new_policy_cmake_version(min_new_policy_version)
        bobui_internal_get_max_new_policy_cmake_version(max_new_policy_version)
        configure_package_config_file(
            "${BOBUI_CMAKE_DIR}/BobUI3rdPartyLibraryConfig.cmake.in"
            "${config_build_dir}/${INSTALL_CMAKE_NAMESPACE}${target}Config.cmake"
            INSTALL_DESTINATION "${config_install_dir}"
        )

        bobui_configure_file(
            OUTPUT "${config_build_dir}/${INSTALL_CMAKE_NAMESPACE}${target}TargetsPrecheck.cmake"
            CONTENT
"
_bobui_internal_should_include_targets(
    TARGETS ${target}
    NAMESPACE ${INSTALL_CMAKE_NAMESPACE}::
    OUT_VAR_SHOULD_SKIP __bobui_${target}_skip_include_targets_file
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

        bobui_install(TARGETS ${target}
            EXPORT "${export_name}"
            RUNTIME DESTINATION ${INSTALL_BINDIR}
            LIBRARY DESTINATION ${INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${INSTALL_LIBDIR}
        )

        bobui_install(EXPORT ${export_name}
            NAMESPACE "${BOBUI_CMAKE_EXPORT_NAMESPACE}::"
            DESTINATION "${config_install_dir}"
        )

        bobui_internal_export_additional_targets_file(
            TARGETS ${target}
            EXPORT_NAME_PREFIX ${INSTALL_CMAKE_NAMESPACE}${target}
            CONFIG_INSTALL_DIR "${config_install_dir}"
        )

        bobui_internal_export_modern_cmake_config_targets_file(
            TARGETS ${target}
            EXPORT_NAME_PREFIX ${INSTALL_CMAKE_NAMESPACE}${target}
            CONFIG_BUILD_DIR "${config_build_dir}"
            CONFIG_INSTALL_DIR "${config_install_dir}"
        )

        set(debug_install_dir "${INSTALL_LIBDIR}")
        if (MINGW)
            set(debug_install_dir "${INSTALL_BINDIR}")
        endif()
        bobui_enable_separate_debug_info(${target} "${debug_install_dir}")
        bobui_internal_install_pdb_files(${target} "${INSTALL_LIBDIR}")
    endif()

    if(BUILD_SHARED_LIBS AND MSVC)
        set_target_properties(${target} PROPERTIES
            INTERPROCEDURAL_OPTIMIZATION OFF
        )
    endif()

    if(BOBUI_GENERATE_SBOM)
        set(sbom_args "")
        list(APPEND sbom_args DEFAULT_SBOM_ENTITY_TYPE BOBUI_THIRD_PARTY_MODULE)

        if(NOT will_install)
            list(APPEND sbom_args NO_INSTALL)
        endif()

        bobui_get_cmake_configurations(configs)
        foreach(config IN LISTS configs)
            _bobui_internal_sbom_append_multi_config_aware_single_arg_option(
                RUNTIME_PATH
                "${INSTALL_BINDIR}"
                "${config}"
                sbom_args
            )
            _bobui_internal_sbom_append_multi_config_aware_single_arg_option(
                LIBRARY_PATH
                "${INSTALL_LIBDIR}"
                "${config}"
                sbom_args
            )
            _bobui_internal_sbom_append_multi_config_aware_single_arg_option(
                ARCHIVE_PATH
                "${INSTALL_LIBDIR}"
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

    bobui_add_list_file_finalizer(bobui_internal_finalize_3rdparty_library ${target})
endfunction()

function(bobui_internal_finalize_3rdparty_library target)
    _bobui_internal_finalize_sbom(${target})
endfunction()

function(bobui_install_3rdparty_library_wrap_config_extra_file target)
    if(TARGET "${target}")
        set(use_bundled "ON")
    else()
        set(use_bundled "OFF")
    endif()

    set(BOBUI_USE_BUNDLED_${target} "${use_bundled}" CACHE INTERNAL "")
    set(extra_cmake_code "set(BOBUI_USE_BUNDLED_${target} ${use_bundled} CACHE INTERNAL \"\")")
    configure_file(
        "${BOBUI_CMAKE_DIR}/BobUIFindWrapConfigExtra.cmake.in"
        "${BOBUI_CONFIG_BUILD_DIR}/${INSTALL_CMAKE_NAMESPACE}/FindWrap${target}ConfigExtra.cmake"
         @ONLY
    )

    bobui_install(FILES
        "${BOBUI_CONFIG_BUILD_DIR}/${INSTALL_CMAKE_NAMESPACE}/FindWrap${target}ConfigExtra.cmake"
        DESTINATION "${BOBUI_CONFIG_INSTALL_DIR}/${INSTALL_CMAKE_NAMESPACE}"
        COMPONENT Devel
    )
endfunction()

# This function implements qmake's bobui_helper_lib MODULE_EXT_HEADERS and MODULE_EXT_HEADERS_DIR features.
# It creates a header-only module exposing a subset or all headers of a 3rd-party library.
function(bobui_internal_add_3rdparty_header_module target)
    set(single_args
        EXTERNAL_HEADERS_DIR
    )
    set(multi_args
        EXTERNAL_HEADERS
    )
    cmake_parse_arguments(PARSE_ARGV 1 arg
        "${option_args}"
        "${single_args}"
        "${multi_args}"
    )
    _bobui_internal_validate_all_args_are_parsed(arg)

    _bobui_internal_forward_function_args(
        FORWARD_PREFIX arg
        FORWARD_OUT_VAR add_module_args
        FORWARD_SINGLE
            EXTERNAL_HEADERS
            EXTERNAL_HEADERS_DIR
    )

    bobui_internal_add_module(${target}
        IS_BOBUI_3RD_PARTY_HEADER_MODULE
        INTERNAL_MODULE
        HEADER_MODULE
        NO_CONFIG_HEADER_FILE
        NO_GENERATE_CPP_EXPORTS
        ${add_module_args}
    )

    set_target_properties(${target} PROPERTIES
        _bobui_module_is_3rdparty_header_library TRUE
        _bobui_module_skip_depends_include TRUE
    )
    set_property(TARGET "${target}"
                 APPEND PROPERTY EXPORT_PROPERTIES _bobui_module_is_3rdparty_header_library)
    set_property(TARGET "${target}"
                 APPEND PROPERTY EXPORT_PROPERTIES _bobui_module_skip_depends_include)
endfunction()
