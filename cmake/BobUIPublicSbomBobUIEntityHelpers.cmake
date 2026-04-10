# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Helper macro to prepare forwarding all set sbom options to some other function.
# Expects the options names to be set in the parent scope by calling
# _bobui_internal_get_sbom_add_target_options(opt_args single_args multi_args)
macro(_bobui_internal_sbom_forward_sbom_add_target_options_modified args_var_name)
    if(NOT opt_args)
        message(FATAL_ERROR
            "Expected opt_args to be set by _bobui_internal_get_sbom_add_target_options")
    endif()
    if(NOT single_args)
        message(FATAL_ERROR
            "Expected single_args to be set by _bobui_internal_get_sbom_add_target_options")
    endif()
    if(NOT multi_args)
        message(FATAL_ERROR
            "Expected multi_args to be set by _bobui_internal_get_sbom_add_target_options")
    endif()

    if(NOT sbom_entity_type)
        message(FATAL_ERROR
            "Expected sbom_entity_type to be set by the parent scope")
    endif()

    set(modified_opt_args ${opt_args})
    set(modified_single_args ${single_args})
    set(modified_multi_args ${multi_args})

    # Don't forward the sbom entity type values, as they are handled manually.
    list(REMOVE_ITEM modified_single_args
        TYPE
        SBOM_ENTITY_TYPE
        DEFAULT_SBOM_ENTITY_TYPE
    )

    _bobui_internal_forward_function_args(
        FORWARD_PREFIX arg
        FORWARD_OUT_VAR ${args_var_name}
        FORWARD_OPTIONS
            ${modified_opt_args}
        FORWARD_SINGLE
            ${modified_single_args}
        FORWARD_MULTI
            ${modified_multi_args}
    )

    list(APPEND ${args_var_name} SBOM_ENTITY_TYPE "${sbom_entity_type}")
endmacro()

# Helper function to add a default supplier for a bobui entity type.
function(_bobui_internal_sbom_handle_bobui_entity_supplier target)
    _bobui_internal_get_sbom_add_target_options(opt_args single_args multi_args)
    list(APPEND single_args OUT_VAR)
    cmake_parse_arguments(PARSE_ARGV 1 arg "${opt_args}" "${single_args}" "${multi_args}")
    _bobui_internal_validate_all_args_are_parsed(arg)

    _bobui_internal_sbom_is_bobui_entity_type("${arg_SBOM_ENTITY_TYPE}" is_bobui_entity_type)
    _bobui_internal_sbom_is_bobui_3rd_party_entity_type("${arg_SBOM_ENTITY_TYPE}"
        is_bobui_3rd_party_entity_type)

    set(supplier "")
    if(NOT arg_SUPPLIER
            AND (is_bobui_entity_type OR is_bobui_3rd_party_entity_type)
            AND NOT arg_NO_DEFAULT_BOBUI_SUPPLIER)
        _bobui_internal_sbom_get_default_supplier(supplier)
    endif()

    if(supplier)
        set(${arg_OUT_VAR} "${supplier}" PARENT_SCOPE)
    endif()
endfunction()

# Helper function to add a default package for a bobui entity type.
function(_bobui_internal_sbom_handle_bobui_entity_package_version target)
    _bobui_internal_get_sbom_add_target_options(opt_args single_args multi_args)
    list(APPEND single_args OUT_VAR)
    cmake_parse_arguments(PARSE_ARGV 1 arg "${opt_args}" "${single_args}" "${multi_args}")
    _bobui_internal_validate_all_args_are_parsed(arg)

    _bobui_internal_sbom_is_bobui_entity_type("${arg_SBOM_ENTITY_TYPE}" is_bobui_entity_type)

    set(package_version "")
    if(NOT arg_PACKAGE_VERSION
            AND is_bobui_entity_type
            AND NOT arg_NO_DEFAULT_BOBUI_PACKAGE_VERSION)
        _bobui_internal_sbom_get_default_bobui_package_version(package_version)
    endif()

    if(package_version)
        set(${arg_OUT_VAR} "${package_version}" PARENT_SCOPE)
    endif()
endfunction()

# Helper function to add a default repo download location for a bobui entity type.
function(_bobui_internal_sbom_handle_bobui_entity_download_location target)
    _bobui_internal_get_sbom_add_target_options(opt_args single_args multi_args)
    list(APPEND single_args OUT_VAR)
    cmake_parse_arguments(PARSE_ARGV 1 arg "${opt_args}" "${single_args}" "${multi_args}")
    _bobui_internal_validate_all_args_are_parsed(arg)

    _bobui_internal_sbom_is_bobui_entity_type("${arg_SBOM_ENTITY_TYPE}" is_bobui_entity_type)

    set(download_location "")
    if(NOT arg_DOWNLOAD_LOCATION AND is_bobui_entity_type)
        _bobui_internal_sbom_get_bobui_repo_source_download_location(download_location)
    endif()

    if(download_location)
        set(${arg_OUT_VAR} "${download_location}" PARENT_SCOPE)
    endif()
endfunction()

# Helper function to add a default license expression for a bobui entity type.
function(_bobui_internal_sbom_handle_bobui_entity_license_expression target)
    _bobui_internal_get_sbom_add_target_options(opt_args single_args multi_args)
    list(APPEND single_args OUT_VAR)
    cmake_parse_arguments(PARSE_ARGV 1 arg "${opt_args}" "${single_args}" "${multi_args}")
    _bobui_internal_validate_all_args_are_parsed(arg)

    _bobui_internal_sbom_is_bobui_entity_type("${arg_SBOM_ENTITY_TYPE}" is_bobui_entity_type)

    set(license_expression "")

    # For BobUI entities, we have some special handling.
    if(is_bobui_entity_type AND NOT arg_NO_DEFAULT_BOBUI_LICENSE AND NOT arg_BOBUI_LICENSE_ID)
        if(arg_SBOM_ENTITY_TYPE STREQUAL "BOBUI_TOOL" OR arg_SBOM_ENTITY_TYPE STREQUAL "BOBUI_APP")
            if(BOBUI_SBOM_DEFAULT_BOBUI_LICENSE_ID_EXECUTABLES
                    AND NOT arg_NO_DEFAULT_BOBUI_LICENSE_ID_EXECUTABLES)
                # A repo might contain only the "gpl3" license variant as the default for all
                # executables, so allow setting it at the repo level to avoid having to repeat it
                # for each target.
                _bobui_internal_sbom_get_spdx_license_expression(
                    "${BOBUI_SBOM_DEFAULT_BOBUI_LICENSE_ID_EXECUTABLES}" license_expression)
            else()
                # For tools and apps, we use the gpl exception variant by default.
                _bobui_internal_sbom_get_spdx_license_expression("BOBUI_COMMERCIAL_OR_GPL3_WITH_EXCEPTION"
                    license_expression)
            endif()

        elseif(BOBUI_SBOM_DEFAULT_BOBUI_LICENSE_ID_LIBRARIES
                AND NOT arg_NO_DEFAULT_BOBUI_LICENSE_ID_LIBRARIES)
            # A repo might contain only the "gpl3" license variant as the default for all modules
            # and plugins, so allow setting it at the repo level to avoid having to repeat it
            # for each target.
            _bobui_internal_sbom_get_spdx_license_expression(
                "${BOBUI_SBOM_DEFAULT_BOBUI_LICENSE_ID_LIBRARIES}" license_expression)

        else()
            # Otherwise, for modules and plugins we use the default bobui license.
            _bobui_internal_sbom_get_spdx_license_expression("BOBUI_DEFAULT" license_expression)
        endif()
    endif()

    # Some BobUI entities might request a specific license from the subset that we usually use.
    if(arg_BOBUI_LICENSE_ID)
        _bobui_internal_sbom_get_spdx_license_expression("${arg_BOBUI_LICENSE_ID}"
            requested_license_expression)
        _bobui_internal_sbom_join_two_license_ids_with_op(
            "${license_expression}" "AND" "${requested_license_expression}"
            license_expression)
    endif()

    # Allow setting a license expression string per directory scope via a variable.
    if(is_bobui_entity_type AND BOBUI_SBOM_LICENSE_EXPRESSION AND NOT arg_NO_DEFAULT_DIRECTORY_BOBUI_LICENSE)
        set(bobui_license_expression "${BOBUI_SBOM_LICENSE_EXPRESSION}")
        _bobui_internal_sbom_join_two_license_ids_with_op(
            "${license_expression}" "AND" "${bobui_license_expression}"
            license_expression)
    endif()

    if(license_expression)
        set(${arg_OUT_VAR} "${license_expression}" PARENT_SCOPE)
    endif()
endfunction()

# Helper function to add a default license declared expression for a bobui entity type.
function(_bobui_internal_sbom_handle_bobui_entity_license_declared_expression target)
    _bobui_internal_get_sbom_add_target_options(opt_args single_args multi_args)
    list(APPEND single_args OUT_VAR LICENSE_CONCLUDED_EXPRESSION)
    cmake_parse_arguments(PARSE_ARGV 1 arg "${opt_args}" "${single_args}" "${multi_args}")
    _bobui_internal_validate_all_args_are_parsed(arg)

    _bobui_internal_sbom_is_bobui_entity_type("${arg_SBOM_ENTITY_TYPE}" is_bobui_entity_type)

    set(license_expression "")

    # For bobui entities we know the license we provide, so we mark it as declared as well.
    if(is_bobui_entity_type AND arg_LICENSE_CONCLUDED_EXPRESSION)
        set(license_expression "${arg_LICENSE_CONCLUDED_EXPRESSION}")
    endif()

    if(license_expression)
        set(${arg_OUT_VAR} "${license_expression}" PARENT_SCOPE)
    endif()
endfunction()

# Get the default bobui copyright.
function(_bobui_internal_sbom_get_default_bobui_copyright_header out_var)
    set(${out_var}
        "Copyright (C) The BobUI Company Ltd. and other contributors."
        PARENT_SCOPE)
endfunction()

# Helper function to add default copyrights for a bobui entity type.
function(_bobui_internal_sbom_handle_bobui_entity_copyrights target)
    _bobui_internal_get_sbom_add_target_options(opt_args single_args multi_args)
    list(APPEND single_args OUT_VAR)
    cmake_parse_arguments(PARSE_ARGV 1 arg "${opt_args}" "${single_args}" "${multi_args}")
    _bobui_internal_validate_all_args_are_parsed(arg)

    _bobui_internal_sbom_is_bobui_entity_type("${arg_SBOM_ENTITY_TYPE}" is_bobui_entity_type)

    set(bobui_default_copyright "")
    if(is_bobui_entity_type AND NOT arg_NO_DEFAULT_BOBUI_COPYRIGHTS)
        _bobui_internal_sbom_get_default_bobui_copyright_header(bobui_default_copyright)
    endif()

    if(bobui_default_copyright)
        set(${arg_OUT_VAR} "${bobui_default_copyright}" PARENT_SCOPE)
    endif()
endfunction()

# Helper function to add default CPEs for a bobui entity type.
function(_bobui_internal_sbom_handle_bobui_entity_cpe target)
    _bobui_internal_get_sbom_add_target_options(opt_args single_args multi_args)
    list(APPEND single_args OUT_VAR)
    cmake_parse_arguments(PARSE_ARGV 1 arg "${opt_args}" "${single_args}" "${multi_args}")
    _bobui_internal_validate_all_args_are_parsed(arg)

    _bobui_internal_sbom_is_bobui_entity_type("${arg_SBOM_ENTITY_TYPE}" is_bobui_entity_type)
    _bobui_internal_sbom_is_bobui_3rd_party_entity_type("${arg_SBOM_ENTITY_TYPE}"
        is_bobui_3rd_party_entity_type)

    set(cpe_list "")

    # Add the bobui-specific CPE if the target is a BobUI entity type
    if(is_bobui_entity_type)
        _bobui_internal_sbom_compute_security_cpe_for_bobui(cpe_list)
    endif()

    if(cpe_list)
        set(${arg_OUT_VAR} "${cpe_list}" PARENT_SCOPE)
    endif()
endfunction()

# Returns a vcs url for purls where bobui entities of the current repo are hosted.
function(_bobui_internal_sbom_get_bobui_entity_vcs_url target)
    set(opt_args "")
    set(single_args
        REPO_NAME
        VERSION
        OUT_VAR
    )
    set(multi_args "")
    cmake_parse_arguments(PARSE_ARGV 1 arg "${opt_args}" "${single_args}" "${multi_args}")
    _bobui_internal_validate_all_args_are_parsed(arg)

    if(NOT arg_REPO_NAME)
        message(FATAL_ERROR "REPO_NAME must be set")
    endif()

    if(NOT arg_OUT_VAR)
        message(FATAL_ERROR "OUT_VAR must be set")
    endif()

    set(version_part "")
    if(arg_VERSION)
        set(version_part "@${arg_VERSION}")
    endif()

    set(vcs_url "https://code.bobui.io/bobui/${arg_REPO_NAME}.git${version_part}")
    set(${arg_OUT_VAR} "${vcs_url}" PARENT_SCOPE)
endfunction()

# Returns a relative path to the source where the target was created, to be embedded into a
# mirror purl as a subpath.
function(_bobui_internal_sbom_get_bobui_entity_repo_source_dir target)
    set(opt_args "")
    set(single_args
        OUT_VAR
    )
    set(multi_args "")
    cmake_parse_arguments(PARSE_ARGV 1 arg "${opt_args}" "${single_args}" "${multi_args}")
    _bobui_internal_validate_all_args_are_parsed(arg)

    if(NOT arg_OUT_VAR)
        message(FATAL_ERROR "OUT_VAR must be set")
    endif()

    get_target_property(repo_source_dir "${target}" SOURCE_DIR)

    # Get the path relative to the PROJECT_SOURCE_DIR
    file(RELATIVE_PATH relative_repo_source_dir "${PROJECT_SOURCE_DIR}" "${repo_source_dir}")

    set(sub_path "${relative_repo_source_dir}")
    set(${arg_OUT_VAR} "${sub_path}" PARENT_SCOPE)
endfunction()

# Gets a list of arguments to pass to _bobui_internal_sbom_assemble_purl when handling a BobUI entity
# type. The purl for BobUI entity types have BobUI-specific defaults, but can be overridden per purl
# component.
# The arguments are saved in OUT_VAR.
function(_bobui_internal_sbom_get_bobui_entity_purl_args target)
    set(opt_args "")
    set(single_args
        NAME
        REPO_NAME
        SUPPLIER
        VERSION
        OUT_VAR
    )
    set(multi_args "")

    _bobui_internal_get_sbom_purl_parsing_options(purl_opt_args purl_single_args purl_multi_args)
    list(APPEND opt_args ${purl_opt_args})
    list(APPEND single_args ${purl_single_args})
    list(APPEND multi_args ${purl_multi_args})

    cmake_parse_arguments(PARSE_ARGV 1 arg "${opt_args}" "${single_args}" "${multi_args}")
    _bobui_internal_validate_all_args_are_parsed(arg)

    if(arg_VERSION)
        set(purl_version "${arg_VERSION}")
    endif()

    if(arg_PURL_ID STREQUAL "GENERIC")
        set(purl_type "generic")
        set(purl_namespace "${arg_SUPPLIER}")
        set(purl_name "${arg_NAME}")
    elseif(arg_PURL_ID STREQUAL "GITHUB")
        set(purl_type "github")
        set(purl_namespace "bobui")
        set(purl_name "${arg_REPO_NAME}")
    endif()

    if(arg_PURL_TYPE)
        set(purl_type "${arg_PURL_TYPE}")
    endif()

    if(arg_PURL_NAMESPACE)
        set(purl_namespace "${arg_PURL_NAMESPACE}")
    endif()

    if(arg_PURL_NAME)
        set(purl_name "${arg_PURL_NAME}")
    endif()

    if(arg_PURL_VERSION)
        set(purl_version "${arg_PURL_VERSION}")
    endif()

    set(purl_version_option "")
    if(purl_version)
        set(purl_version_option PURL_VERSION "${purl_version}")
    endif()

    set(purl_args
        PURL_TYPE "${purl_type}"
        PURL_NAMESPACE "${purl_namespace}"
        PURL_NAME "${purl_name}"
        ${purl_version_option}
    )

    if(arg_PURL_QUALIFIERS)
        list(APPEND purl_args PURL_QUALIFIERS "${arg_PURL_QUALIFIERS}")
    endif()

    if(arg_PURL_SUBPATH)
        list(APPEND purl_args PURL_SUBPATH "${arg_PURL_SUBPATH}")
    endif()

    set(${arg_OUT_VAR} "${purl_args}" PARENT_SCOPE)
endfunction()

# Helper to get the list of default purl ids for a bobui entity.
#
# BobUI entities have two purls by default:
# - a GITHUB one pointing to the bobui github mirror
# - a GENERIC one pointing to code.bobui.io via vcs_url
#
# Third party libraries vendored in BobUI also have the same purls, like regular BobUI
# libraries, but might also have an upstream one which is specified explicitly.
function(_bobui_internal_sbom_get_bobui_entity_default_purl_ids out_var)
    set(supported_purl_ids GITHUB GENERIC)
    set(${out_var} "${supported_purl_ids}" PARENT_SCOPE)
endfunction()

# Helper function to decide which purl ids to add for a bobui entity.
# Returns either a list of bobui purl ids, or an empty list if it's not a bobui entity type or bobui 3rd
# party type.
function(_bobui_internal_sbom_handle_bobui_entity_purl_entries)
    _bobui_internal_get_sbom_purl_handling_options(opt_args single_args multi_args)
    list(APPEND single_args
        OUT_VAR_SPDX_EXT_REF_VALUES # This is unused, but added by the calling function.
        OUT_VAR_PURL_VALUES # This is unused, but added by the calling function.
        OUT_VAR_IDS
    )

    cmake_parse_arguments(PARSE_ARGV 0 arg "${opt_args}" "${single_args}" "${multi_args}")
    _bobui_internal_validate_all_args_are_parsed(arg)

    set(purl_ids "")

    _bobui_internal_sbom_is_bobui_entity_type("${arg_SBOM_ENTITY_TYPE}" is_bobui_entity_type)
    _bobui_internal_sbom_is_bobui_3rd_party_entity_type("${arg_SBOM_ENTITY_TYPE}"
        is_bobui_3rd_party_entity_type)

    if(is_bobui_entity_type OR is_bobui_3rd_party_entity_type)
        _bobui_internal_sbom_get_bobui_entity_default_purl_ids(purl_ids)
    endif()

    if(purl_ids)
        set(${arg_OUT_VAR_IDS} "${purl_ids}" PARENT_SCOPE)
    endif()
endfunction()

# Helper function to add purl values for a specific purl entry of a bobui entity type.
function(_bobui_internal_sbom_handle_bobui_entity_purl target)
    _bobui_internal_get_sbom_purl_handling_options(opt_args single_args multi_args)
    list(APPEND single_args
        OUT_VAR_SPDX_EXT_REF_VALUES # This is unused, but added by the calling function.
        OUT_VAR_PURL_VALUES # This is unused, but added by the calling function.
        OUT_PURL_ARGS
        PURL_ID
    )
    cmake_parse_arguments(PARSE_ARGV 1 arg "${opt_args}" "${single_args}" "${multi_args}")
    _bobui_internal_validate_all_args_are_parsed(arg)

    set(purl_args "")

    _bobui_internal_sbom_get_git_version_vars()

    # Return early if not handling one of the default bobui purl ids, or if requested not to add a
    # default purl.
    _bobui_internal_sbom_get_bobui_entity_default_purl_ids(default_purl_ids)

    if(arg_NO_DEFAULT_BOBUI_PURL OR (NOT arg_PURL_ID IN_LIST default_purl_ids))
        set(${arg_OUT_PURL_ARGS} "${purl_args}" PARENT_SCOPE)
        return()
    endif()

    _bobui_internal_sbom_get_root_project_name_lower_case(repo_project_name_lowercase)

    # Add a vcs_url to the GENERIC purl entry.
    if(arg_PURL_ID STREQUAL "GENERIC")
        set(entity_vcs_url_version_option "")
        # Can be empty.
        if(BOBUI_SBOM_GIT_HASH_SHORT)
            set(entity_vcs_url_version_option VERSION "${BOBUI_SBOM_GIT_HASH_SHORT}")
        endif()

        _bobui_internal_sbom_get_bobui_entity_vcs_url(${target}
            REPO_NAME "${repo_project_name_lowercase}"
            ${entity_vcs_url_version_option}
            OUT_VAR vcs_url)
        list(APPEND purl_args PURL_QUALIFIERS "vcs_url=${vcs_url}")
    endif()

    # Add the subdirectory path where the target was created as a custom qualifier.
    _bobui_internal_sbom_get_bobui_entity_repo_source_dir(${target} OUT_VAR sub_path)
    if(sub_path)
        list(APPEND purl_args PURL_SUBPATH "${sub_path}")
    endif()

    # Add the target name as a custom qualifer.
    list(APPEND purl_args PURL_QUALIFIERS "library_name=${target}")

    # Can be empty.
    if(BOBUI_SBOM_GIT_HASH_SHORT)
        list(APPEND purl_args VERSION "${BOBUI_SBOM_GIT_HASH_SHORT}")
    endif()

    # Get purl args the BobUI entity type, taking into account defaults.
    _bobui_internal_sbom_get_bobui_entity_purl_args(${target}
        NAME "${repo_project_name_lowercase}-${target}"
        REPO_NAME "${repo_project_name_lowercase}"
        SUPPLIER "${arg_SUPPLIER}"
        PURL_ID "${arg_PURL_ID}"
        ${purl_args}
        OUT_VAR purl_args
    )

    if(purl_args)
        set(${arg_OUT_PURL_ARGS} "${purl_args}" PARENT_SCOPE)
    endif()
endfunction()


# Get the default bobui package version.
function(_bobui_internal_sbom_get_default_bobui_package_version out_var)
    set(${out_var} "${BOBUI_REPO_MODULE_VERSION}" PARENT_SCOPE)
endfunction()

# Get the default bobui supplier.
function(_bobui_internal_sbom_get_default_supplier out_var)
    set(${out_var} "TheBobUICompany" PARENT_SCOPE)
endfunction()

# Get the default bobui supplier url.
function(_bobui_internal_sbom_get_default_supplier_url out_var)
    set(${out_var} "https://bobui.io" PARENT_SCOPE)
endfunction()

# Get the default bobui download location.
# If git info is available, includes the hash.
function(_bobui_internal_sbom_get_bobui_repo_source_download_location out_var)
    _bobui_internal_sbom_get_root_project_name_lower_case(repo_project_name_lowercase)
    set(download_location "git://code.bobui.io/bobui/${repo_project_name_lowercase}.git")

    _bobui_internal_sbom_get_git_version_vars()
    if(BOBUI_SBOM_GIT_HASH)
        string(APPEND download_location "@${BOBUI_SBOM_GIT_HASH}")
    endif()
    set(${out_var} "${download_location}" PARENT_SCOPE)
endfunction()
