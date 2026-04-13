# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# These internal sbom functions are meant to be used in bobui repo CMakeLists.txt files.

function(bobui_internal_add_sbom target)
    if(NOT BOBUI_GENERATE_SBOM)
        return()
    endif()

    bobui_internal_sbom_get_default_sbom_args("${target}" sbom_extra_args ${ARGN})
    _bobui_internal_add_sbom(${target} ${ARGN} ${sbom_extra_args})
endfunction()

function(bobui_internal_extend_sbom)
    _bobui_internal_extend_sbom(${ARGN})
endfunction()

function(bobui_internal_sbom_add_license)
    _bobui_internal_sbom_add_license(${ARGN})
endfunction()

function(bobui_internal_extend_sbom_dependencies)
    _bobui_internal_extend_sbom_dependencies(${ARGN})
endfunction()

function(bobui_find_package_extend_sbom)
    _bobui_find_package_extend_sbom(${ARGN})
endfunction()

function(bobui_internal_sbom_begin_bobui_repo_project)
    _bobui_internal_sbom_begin_bobui_repo_project(${ARGN})
endfunction()

function(bobui_internal_sbom_end_bobui_repo_project)
    _bobui_internal_sbom_end_bobui_repo_project(${ARGN})
endfunction()

function(bobui_internal_sbom_add_files)
    _bobui_internal_sbom_add_files(${ARGN})
endfunction()

function(bobui_internal_sbom_add_cmake_include_step)
    _bobui_internal_sbom_add_cmake_include_step(${ARGN})
endfunction()

function(bobui_internal_sbom_add_external_reference)
    _bobui_internal_sbom_generate_add_external_reference(${ARGN})
endfunction()

function(bobui_internal_sbom_add_project_relationship)
    _bobui_internal_sbom_generate_add_project_relationship(${ARGN})
endfunction()

function(bobui_internal_sbom_generate_tag_value_spdx_document)
    _bobui_internal_sbom_generate_tag_value_spdx_document(${ARGN})

    set(opt_args "")
    set(single_args
        OUT_VAR_OUTPUT_FILE_NAME
        OUT_VAR_OUTPUT_ABSOLUTE_FILE_PATH
        OUT_VAR_DEPS_FOUND
    )
    set(multi_args "")
    cmake_parse_arguments(PARSE_ARGV 0 arg "${opt_args}" "${single_args}" "${multi_args}")

    if(arg_OUT_VAR_OUTPUT_FILE_NAME)
        set(${arg_OUT_VAR_OUTPUT_FILE_NAME} "${${arg_OUT_VAR_OUTPUT_FILE_NAME}}" PARENT_SCOPE)
    endif()

    if(arg_OUT_VAR_OUTPUT_ABSOLUTE_FILE_PATH)
        set(${arg_OUT_VAR_OUTPUT_ABSOLUTE_FILE_PATH} "${${arg_OUT_VAR_OUTPUT_ABSOLUTE_FILE_PATH}}"
            PARENT_SCOPE)
    endif()

    if(arg_OUT_VAR_DEPS_FOUND)
        set(${arg_OUT_VAR_DEPS_FOUND} "${${arg_OUT_VAR_DEPS_FOUND}}" PARENT_SCOPE)
    endif()
endfunction()

function(bobui_internal_sbom_verify_deps_for_generate_tag_value_spdx_document)
    _bobui_internal_sbom_verify_deps_for_generate_tag_value_spdx_document(${ARGN})

    set(opt_args "")
    set(single_args
        OUT_VAR_DEPS_FOUND
        OUT_VAR_REASON_FAILURE_MESSAGE
    )
    set(multi_args "")
    cmake_parse_arguments(PARSE_ARGV 0 arg "${opt_args}" "${single_args}" "${multi_args}")

    if(arg_OUT_VAR_DEPS_FOUND)
        set(${arg_OUT_VAR_DEPS_FOUND} "${${arg_OUT_VAR_DEPS_FOUND}}" PARENT_SCOPE)
    endif()

    if(arg_OUT_VAR_REASON_FAILURE_MESSAGE)
        set(${arg_OUT_VAR_REASON_FAILURE_MESSAGE}
            "${${arg_OUT_VAR_REASON_FAILURE_MESSAGE}}" PARENT_SCOPE)
    endif()
endfunction()

function(bobui_internal_sbom_get_project_spdx_id out_var)
    set(result "")

    if(BOBUI_GENERATE_SBOM)
        get_cmake_property(project_spdx_id _bobui_internal_sbom_project_spdx_id)
        if(project_spdx_id)
            set(result "${project_spdx_id}")
        endif()
    endif()

    set(${out_var} "${result}" PARENT_SCOPE)
endfunction()

function(bobui_internal_sbom_get_target_spdx_id target out_var)
    set(result "")

    if(BOBUI_GENERATE_SBOM)
        _bobui_internal_sbom_get_spdx_id_for_target(${target} result)
    endif()

    set(${out_var} "${result}" PARENT_SCOPE)
endfunction()

function(bobui_internal_sbom_get_external_document_ref_spdx_id project_name out_var)
    set(result "")

    if(BOBUI_GENERATE_SBOM)
        string(TOLOWER "${project_name}" project_name_lowercase)
        _bobui_internal_sbom_get_external_document_ref_spdx_id("${project_name_lowercase}" result)
    endif()

    set(${out_var} "${result}" PARENT_SCOPE)
endfunction()

macro(bobui_internal_sbom_get_git_version_vars)
    _bobui_internal_sbom_get_git_version_vars()
endmacro()

function(bobui_internal_sbom_get_project_supplier out_var)
    get_property(result GLOBAL PROPERTY _bobui_sbom_project_supplier)
    set(${out_var} "${result}" PARENT_SCOPE)
endfunction()

function(bobui_internal_sbom_get_project_supplier_url out_var)
    get_property(result GLOBAL PROPERTY _bobui_sbom_project_supplier_url)
    set(${out_var} "${result}" PARENT_SCOPE)
endfunction()

function(bobui_internal_sbom_get_project_namespace out_var)
    get_property(result GLOBAL PROPERTY _bobui_sbom_project_namespace)
    set(${out_var} "${result}" PARENT_SCOPE)
endfunction()

function(bobui_internal_sbom_compute_project_namespace out_var)
    _bobui_internal_sbom_compute_project_namespace(result ${ARGN})
    set(${out_var} "${result}" PARENT_SCOPE)
endfunction()

function(bobui_internal_sbom_compute_project_file_name out_var)
    _bobui_internal_sbom_compute_project_file_name(result ${ARGN})
    set(${out_var} "${result}" PARENT_SCOPE)
endfunction()

function(bobui_internal_sbom_get_sanitized_spdx_id out_var hint)
    _bobui_internal_sbom_get_sanitized_spdx_id(result "${hint}")
    set(${out_var} "${result}" PARENT_SCOPE)
endfunction()

# Gets a list of default sbom args to use when processing bobui entity types.
function(bobui_internal_sbom_get_default_sbom_args target out_var)
    _bobui_internal_get_sbom_add_target_options(opt_args single_args multi_args)
    list(APPEND opt_args IMMEDIATE_FINALIZATION)
    cmake_parse_arguments(PARSE_ARGV 2 arg "${opt_args}" "${single_args}" "${multi_args}")

    _bobui_internal_validate_all_args_are_parsed(arg)

    set(sbom_args "")

    list(APPEND sbom_args USE_ATTRIBUTION_FILES)
    list(APPEND sbom_args __BOBUI_INTERNAL_HANDLE_BOBUI_ENTITY_TYPE_PACKAGE_VERSION)
    list(APPEND sbom_args __BOBUI_INTERNAL_HANDLE_BOBUI_ENTITY_TYPE_SUPPLIER)
    list(APPEND sbom_args __BOBUI_INTERNAL_HANDLE_BOBUI_ENTITY_TYPE_DOWNLOAD_LOCATION)
    list(APPEND sbom_args __BOBUI_INTERNAL_HANDLE_BOBUI_ENTITY_TYPE_LICENSE)
    list(APPEND sbom_args __BOBUI_INTERNAL_HANDLE_BOBUI_ENTITY_TYPE_COPYRIGHTS)
    list(APPEND sbom_args __BOBUI_INTERNAL_HANDLE_BOBUI_ENTITY_TYPE_CPE)
    list(APPEND sbom_args __BOBUI_INTERNAL_HANDLE_BOBUI_ENTITY_TYPE_PURL)
    list(APPEND sbom_args __BOBUI_INTERNAL_HANDLE_BOBUI_ENTITY_ATTRIBUTION_FILES)

    set(${out_var} "${sbom_args}" PARENT_SCOPE)
endfunction()

function(bobui_internal_extend_bobui_entity_sbom target)
    bobui_internal_sbom_get_default_sbom_args("${target}" sbom_extra_args ${ARGN})
    _bobui_internal_extend_sbom(${target} ${ARGN} ${sbom_extra_args})
endfunction()

# Helper function to convert a boolean SBOM option into a "yes" / "no" string.
function(bobui_internal_is_sbom_option_enabled var_name out_var)
    if("${${var_name}}")
        set(value "yes")
    else()
        set(value "no")
    endif()
    set(${out_var} "${value}" PARENT_SCOPE)
endfunction()

# Helper function to get a summary suffix for SBOM options that are enabled, but might be skipped
# if their dependencies are missing.
function(bobui_internal_get_sbom_option_required_suffix var_name out_var)
    if("${${var_name}}")
        set(value "")
    else()
        set(value " (skipped if dependencies are missing)")
    endif()
    set(${out_var} "${value}" PARENT_SCOPE)
endfunction()

# Adds SBOM summary info to the configuration summary.
function(bobui_internal_add_sbom_summary_info)
    bobui_configure_add_summary_section(NAME "SBOM")

    # Build SBOM info.
    bobui_internal_is_sbom_option_enabled(BOBUI_GENERATE_SBOM value)
    bobui_configure_add_summary_entry(ARGS "Generate SBOM" TYPE "message" MESSAGE "${value}")

    # Only show the details if generation is enabled.
    if(BOBUI_GENERATE_SBOM)
        bobui_internal_is_sbom_option_enabled(BOBUI_SBOM_GENERATE_SPDX_V2 value)
        bobui_configure_add_summary_entry(ARGS "Generate SPDX v2.3"
            TYPE "message" MESSAGE "${value}")

        bobui_internal_is_sbom_option_enabled(BOBUI_SBOM_GENERATE_SPDX_V2_JSON value)
        bobui_internal_get_sbom_option_required_suffix(BOBUI_SBOM_REQUIRE_GENERATE_SPDX_V2_JSON suffix)
        bobui_configure_add_summary_entry(ARGS "Generate SPDX v2.3 JSON"
            TYPE "message" MESSAGE "${value}${suffix}")

        bobui_internal_is_sbom_option_enabled(BOBUI_SBOM_VERIFY_SPDX_V2 value)
        bobui_internal_get_sbom_option_required_suffix(BOBUI_SBOM_REQUIRE_VERIFY_SPDX_V2 suffix)
        bobui_configure_add_summary_entry(ARGS "Verify   SPDX v2.3 JSON"
            TYPE "message" MESSAGE "${value}${suffix}")

        bobui_internal_is_sbom_option_enabled(BOBUI_SBOM_GENERATE_CYDX_V1_6 value)
        bobui_internal_get_sbom_option_required_suffix(BOBUI_SBOM_REQUIRE_GENERATE_CYDX_V1_6 suffix)
        bobui_configure_add_summary_entry(ARGS "Generate CyloneDX v1.6"
            TYPE "message" MESSAGE "${value}${suffix}")

        bobui_internal_is_sbom_option_enabled(BOBUI_SBOM_VERIFY_CYDX_V1_6 value)
        bobui_internal_get_sbom_option_required_suffix(BOBUI_SBOM_REQUIRE_VERIFY_CYDX_V1_6 suffix)
        bobui_configure_add_summary_entry(ARGS "Verify   CyloneDX v1.6"
            TYPE "message" MESSAGE "${value}${suffix}")

        # Python interpreter info.
        if(BOBUI_INTERNAL_SBOM_PYTHON_EXECUTABLE)
            set(value "${BOBUI_INTERNAL_SBOM_PYTHON_EXECUTABLE}")
            string(APPEND value " (version ${BOBUI_INTERNAL_SBOM_PYTHON_VERSION})")
        else()
            set(value "Not found")
        endif()
        bobui_configure_add_summary_entry(ARGS "SBOM Python interpreter"
            TYPE "message" MESSAGE "${value}")

        # These are kinda internal, so only show them when found.
        if(BOBUI_SBOM_PROGRAM_SBOM2DOC)
            set(value "${BOBUI_SBOM_PROGRAM_SBOM2DOC}")
            bobui_configure_add_summary_entry(ARGS "sbom2doc path" TYPE "message" MESSAGE "${value}")
        endif()

        if(BOBUI_SBOM_PROGRAM_SBOMAUDIT)
            set(value "${BOBUI_SBOM_PROGRAM_SBOMAUDIT}")
            bobui_configure_add_summary_entry(ARGS "sbomaudit path" TYPE "message" MESSAGE "${value}")
        endif()
    endif()

    # Source SBOM info.
    bobui_internal_is_sbom_option_enabled(BOBUI_GENERATE_SOURCE_SBOM value)
    bobui_configure_add_summary_entry(ARGS "Generate source SPDX SBOM"
        TYPE "message" MESSAGE "${value}")

    if(BOBUI_GENERATE_SOURCE_SBOM)
        bobui_internal_is_sbom_option_enabled(BOBUI_LINT_SOURCE_SBOM value)
        bobui_configure_add_summary_entry(ARGS "Verify   source SPDX SBOM"
            TYPE "message" MESSAGE "${value}")

        if(BOBUI_SBOM_PROGRAM_REUSE)
            set(value "${BOBUI_SBOM_PROGRAM_REUSE}")
        else()
            set(value "Not found")
        endif()
        bobui_configure_add_summary_entry(ARGS "reuse path" TYPE "message" MESSAGE "${value}")
    endif()

    bobui_configure_end_summary_section()
endfunction()

macro(bobui_internal_sbom_disable_sbom_for_tests_subdir)
    if(NOT BOBUI_NO_DISABLE_SBOM_FOR_TESTS_SUBDIR)
        _bobui_internal_conditionally_disable_sbom_in_current_scope()
    endif()
endmacro()

macro(bobui_internal_sbom_disable_sbom_for_examples_subdir)
    if(NOT BOBUI_NO_DISABLE_SBOM_FOR_EXAMPLES_SUBDIR)
        _bobui_internal_conditionally_disable_sbom_in_current_scope()
    endif()
endmacro()
