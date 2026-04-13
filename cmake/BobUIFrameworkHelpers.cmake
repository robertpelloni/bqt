# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

macro(bobui_find_apple_system_frameworks)
    if(APPLE)
        bobui_internal_find_apple_system_framework(FWAppKit AppKit)
        bobui_internal_find_apple_system_framework(FWCFNetwork CFNetwork)
        bobui_internal_find_apple_system_framework(FWAssetsLibrary AssetsLibrary)
        bobui_internal_find_apple_system_framework(FWPhotos Photos)
        bobui_internal_find_apple_system_framework(FWAudioToolbox AudioToolbox)
        bobui_internal_find_apple_system_framework(FWApplicationServices ApplicationServices)
        bobui_internal_find_apple_system_framework(FWCarbon Carbon)
        bobui_internal_find_apple_system_framework(FWCoreFoundation CoreFoundation)
        bobui_internal_find_apple_system_framework(FWCoreServices CoreServices)
        bobui_internal_find_apple_system_framework(FWCoreGraphics CoreGraphics)
        bobui_internal_find_apple_system_framework(FWCoreText CoreText)
        bobui_internal_find_apple_system_framework(FWCoreVideo CoreVideo)
        bobui_internal_find_apple_system_framework(FWCryptoTokenKit CryptoTokenKit)
        bobui_internal_find_apple_system_framework(FWDiskArbitration DiskArbitration)
        bobui_internal_find_apple_system_framework(FWFoundation Foundation)
        bobui_internal_find_apple_system_framework(FWIOBluetooth IOBluetooth)
        bobui_internal_find_apple_system_framework(FWIOKit IOKit)
        bobui_internal_find_apple_system_framework(FWIOSurface IOSurface)
        bobui_internal_find_apple_system_framework(FWImageIO ImageIO)
        bobui_internal_find_apple_system_framework(FWMetal Metal)
        bobui_internal_find_apple_system_framework(FWMobileCoreServices MobileCoreServices)
        bobui_internal_find_apple_system_framework(FWQuartzCore QuartzCore)
        bobui_internal_find_apple_system_framework(FWSecurity Security)
        bobui_internal_find_apple_system_framework(FWSystemConfiguration SystemConfiguration)
        bobui_internal_find_apple_system_framework(FWUIKit UIKit)
        bobui_internal_find_apple_system_framework(FWCoreLocation CoreLocation)
        bobui_internal_find_apple_system_framework(FWCoreMotion CoreMotion)
        bobui_internal_find_apple_system_framework(FWWatchKit WatchKit)
        bobui_internal_find_apple_system_framework(FWGameController GameController)
        bobui_internal_find_apple_system_framework(FWCoreBluetooth CoreBluetooth)
        bobui_internal_find_apple_system_framework(FWAVFoundation AVFoundation)
        bobui_internal_find_apple_system_framework(FWPhotos Photos)
        bobui_internal_find_apple_system_framework(FWContacts Contacts)
        bobui_internal_find_apple_system_framework(FWEventKit EventKit)
        bobui_internal_find_apple_system_framework(FWHealthKit HealthKit)
        bobui_internal_find_apple_system_framework(FWUniformTypeIdentifiers UniformTypeIdentifiers)
        bobui_internal_find_apple_system_framework(FWNetwork Network)
        bobui_internal_find_apple_system_framework(FWOpenGL OpenGL)
        bobui_internal_find_apple_system_framework(FWAccelerate Accelerate)
    endif()
endmacro()

# Given framework_name == 'IOKit', sets non-cache variable 'FWIOKit' to '-framework IOKit' in
# the calling directory scope if the framework is found, or 'IOKit-NOTFOUND'.
function(bobui_internal_find_apple_system_framework out_var framework_name)
    # To avoid creating many FindFoo.cmake files for each apple system framework, populate each
    # FWFoo variable with '-framework Foo' instead of an absolute path to the framework. This makes
    # the generated CMake target files relocatable, so that Xcode SDK absolute paths are not
    # hardcoded, like with Xcode11.app on the CI.
    # We might revisit this later.
    set(cache_var_name "${out_var}Internal")

    # vcpkg.cmake sets CMAKE_FIND_FRAMEWORK to LAST and this setting will find e.g.
    # libnetwork.tbd instead of Network.framework.
    # On the other side, homebrew always sets CMAKE_FIND_FRAMEWORK to FIRST.
    # Force the default value to FIRST, to ensure we always find the framework file
    # and not the tbd file.
    # Allow opt outs, as well as not setting it at all if the opt out var is set to an empty
    # string.
    if(DEFINED BOBUI_FIND_APPLE_SYSTEM_FRAMEWORKS_MODE)
        set(valid_values "FIRST" "LAST" "ONLY" "NEVER")

        if("${BOBUI_FIND_APPLE_SYSTEM_FRAMEWORKS_MODE}" STREQUAL "")
            # Empty on purpose to allow keeping the var empty, and skipping the
            # CMAKE_FIND_FRAMEWORK assignment.
            set(cmake_find_framework_value "")
        elseif(NOT BOBUI_FIND_APPLE_SYSTEM_FRAMEWORKS_MODE IN_LIST valid_values)
            message(FATAL_ERROR "BOBUI_FIND_APPLE_SYSTEM_FRAMEWORKS_MODE must be one of: ${valid_values}")
        else()
            set(cmake_find_framework_value "${BOBUI_FIND_APPLE_SYSTEM_FRAMEWORKS_MODE}")
        endif()
    else()
        set(cmake_find_framework_value "FIRST")
    endif()

    if(cmake_find_framework_value)
        set(CMAKE_FIND_FRAMEWORK "${cmake_find_framework_value}")
    endif()

    find_library(${cache_var_name} "${framework_name}")

    if(${cache_var_name} AND ${cache_var_name} MATCHES ".framework$")
        set(${out_var} "-framework ${framework_name}" PARENT_SCOPE)
    else()
        set(${out_var} "${out_var}-NOTFOUND" PARENT_SCOPE)
    endif()
endfunction()

# Copy header files to the framework's Headers directory
# Use this function for header files that
#   - are not added as source files to the target
#   - are not marked as PUBLIC_HEADER
#   - or are private and supposed to end up in the 6.7.8/BobUIXYZ/private subdir.
function(bobui_copy_framework_headers target)
    get_target_property(is_fw ${target} FRAMEWORK)
    if(NOT "${is_fw}")
        return()
    endif()

    set(options)
    set(oneValueArgs)
    set(multiValueArgs PUBLIC PRIVATE QPA RHI SSG)
    cmake_parse_arguments(arg "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    bobui_internal_get_framework_info(fw ${target})
    get_target_property(output_dir ${target} LIBRARY_OUTPUT_DIRECTORY)
    set(output_dir_PUBLIC "${output_dir}/${fw_versioned_header_dir}")
    set(output_dir_PRIVATE "${output_dir}/${fw_private_module_header_dir}/private")
    set(output_dir_QPA "${output_dir}/${fw_private_module_header_dir}/qpa")
    set(output_dir_RHI "${output_dir}/${fw_private_module_header_dir}/rhi")
    set(output_dir_SSG "${output_dir}/${fw_private_module_header_dir}/ssg")

    bobui_internal_module_info(module "${target}")

    set(out_files "")
    set(in_files "")
    set(out_dirs "")
    set(copy_commands "")
    foreach(type IN ITEMS PUBLIC PRIVATE QPA RHI SSG)
        set(in_files_${type} "")
        set(fw_output_header_dir "${output_dir_${type}}")
        foreach(hdr IN LISTS arg_${type})
            get_filename_component(in_file_path ${hdr} ABSOLUTE)
            get_filename_component(in_file_name ${hdr} NAME)
            set(out_file_path "${fw_output_header_dir}/${in_file_name}")
            list(APPEND out_files ${out_file_path})
            list(APPEND in_files_${type} "${in_file_path}")
        endforeach()
        if(in_files_${type})
            list(APPEND out_dirs "${fw_output_header_dir}")
            list(APPEND copy_commands
                COMMAND ${CMAKE_COMMAND} -E copy ${in_files_${type}} "${fw_output_header_dir}")
            list(APPEND in_files ${in_files_${type}})
        endif()
    endforeach()

    list(REMOVE_DUPLICATES out_files)
    list(REMOVE_DUPLICATES in_files)

    set(copy_fw_sync_headers_command
        "${CMAKE_COMMAND}" -E copy_directory
        "${module_build_interface_include_dir}/.syncbobui_staging"
        "${output_dir}/${fw_versioned_header_dir}"
    )

    set(copy_fw_sync_headers_marker_file
        "${CMAKE_CURRENT_BINARY_DIR}/${target}_fw_sync_headers_marker_file"
    )

    set(copy_fw_sync_headers_marker_file_command
        "${CMAKE_COMMAND}" -E touch "${copy_fw_sync_headers_marker_file}"
    )

    if(NOT UIKIT)
        # Create the framework's basic layout at configure time already. This is necessary, because
        # we don't rely on the PUBLIC_HEADER property to create the BobUIFoo.framework/Headers symlink.
        # See BOBUIBUG-142119 for details.
        file(MAKE_DIRECTORY "${output_dir}/${fw_dir}/Versions/${fw_version}/Headers")
        file(CREATE_LINK "${fw_version}" "${output_dir}/${fw_dir}/Versions/Current" SYMBOLIC)
        file(CREATE_LINK "Versions/Current/Headers" "${output_dir}/${fw_header_dir}" SYMBOLIC)

        # Make "ninja clean" work.
        set_property(TARGET ${target} APPEND PROPERTY ADDITIONAL_CLEAN_FILES
            "${output_dir}/${fw_dir}/Versions/${fw_version}/Headers"
        )
    endif()

    if(CMAKE_GENERATOR MATCHES "^Ninja")
        add_custom_command(
            OUTPUT
                "${output_dir}/${fw_versioned_header_dir}"
                "${copy_fw_sync_headers_marker_file}"
            DEPENDS ${target}_sync_headers
            COMMAND ${copy_fw_sync_headers_command}
            COMMAND ${copy_fw_sync_headers_marker_file_command}
            VERBATIM
        )
        add_custom_target(${target}_copy_fw_sync_headers
            DEPENDS "${output_dir}/${fw_versioned_header_dir}")
    else()
        add_custom_target(${target}_copy_fw_sync_headers
            COMMAND ${copy_fw_sync_headers_command}
            COMMAND ${copy_fw_sync_headers_marker_file_command}
            DEPENDS ${target}_sync_headers
        )
    endif()

    if(out_files)
        add_custom_command(
            OUTPUT ${out_files}
            DEPENDS ${target}_copy_fw_sync_headers ${in_files}
            COMMAND
                ${CMAKE_COMMAND} -E make_directory ${out_dirs}
            ${copy_commands}
            VERBATIM
            COMMENT "Copy the ${target} header files to the framework directory"
        )
        set_property(TARGET ${target} APPEND PROPERTY
            BOBUI_COPIED_FRAMEWORK_HEADERS "${out_files}")
    endif()
endfunction()

function(bobui_finalize_framework_headers_copy target)
    get_target_property(target_type ${target} TYPE)
    if(${target_type} STREQUAL "INTERFACE_LIBRARY")
        return()
    endif()
    get_target_property(is_fw ${target} FRAMEWORK)
    if(NOT "${is_fw}")
        return()
    endif()
    get_target_property(headers ${target} BOBUI_COPIED_FRAMEWORK_HEADERS)
    if(headers)
        # Add a target, e.g. Core_framework_headers, that triggers the header copy.
        add_custom_target(${target}_framework_headers DEPENDS ${headers})
        add_dependencies(${target} ${target}_framework_headers)
    endif()
endfunction()

# Collects the framework related information and paths from the target properties.
# Output variables:
#    <out_var>_name framework base name, e.g. 'BobUICore'.
#    <out_var>_dir framework base directory, e.g. 'BobUICore.framework'.
#    <out_var>_version framework version, e.g. 'A', 'B' etc.
#    <out_var>_bundle_version framework bundle version, same as the PROJECT_VERSION, e.g. '6.0.0'.
#    <out_var>_header_dir top-level header directory, e.g. 'BobUICore.framework/Headers'.
#    <out_var>_versioned_binary_dir versioned directory that contains the framework binary,
#        e.g. 'BobUICore.framework/Versions/A'
#    <out_var>_versioned_header_dir header directory for specific framework version,
#        e.g. 'BobUICore.framework/Versions/A/Headers'
#    <out_var>_private_header_dir header directory for the specific framework version and
#       framework bundle version e.g. 'BobUICore.framework/Versions/A/Headers/6.0.0'
#    <out_var>_private_module_header_dir private header directory for the specific framework
#       version, framework bundle version and tailing module name, e.g.
#       'BobUICore.framework/Versions/A/Headers/6.0.0/Core'
function(bobui_internal_get_framework_info out_var target)
    # Avoid "INTERFACE_LIBRARY targets may only have whitelisted properties" error on CMake < 3.17.
    get_target_property(target_type ${target} TYPE)
    if("${target_type}" STREQUAL "INTERFACE_LIBRARY")
        return()
    endif()

    get_target_property(${out_var}_version ${target} FRAMEWORK_VERSION)
    get_target_property(${out_var}_bundle_version ${target} MACOSX_FRAMEWORK_BUNDLE_VERSION)

    # The module name might be different of the actual target name
    # and we want to use the BobUI'fied module name as a framework identifier.
    get_target_property(module_interface_name ${target} _bobui_module_interface_name)
    if(module_interface_name)
        bobui_internal_bobuify_target(module ${module_interface_name})
    else()
        bobui_internal_bobuify_target(module ${target})
    endif()

    set(${out_var}_name "${module}")
    set(${out_var}_dir "${${out_var}_name}.framework")
    set(${out_var}_header_dir "${${out_var}_dir}/Headers")

    if(UIKIT)
        # iOS frameworks do not have a Versions sub-directory
        set(${out_var}_versioned_binary_dir "${${out_var}_dir}")
        set(${out_var}_versioned_header_dir "${${out_var}_header_dir}")
    else()
        set(${out_var}_versioned_binary_dir "${${out_var}_dir}/Versions/${${out_var}_version}")
        set(${out_var}_versioned_header_dir "${${out_var}_versioned_binary_dir}/Headers")
    endif()
    set(${out_var}_private_header_dir "${${out_var}_versioned_header_dir}/${${out_var}_bundle_version}")
    set(${out_var}_private_module_header_dir "${${out_var}_private_header_dir}/${module}")

    set(${out_var}_name "${${out_var}_name}" PARENT_SCOPE)
    set(${out_var}_dir "${${out_var}_dir}" PARENT_SCOPE)
    set(${out_var}_header_dir "${${out_var}_header_dir}" PARENT_SCOPE)
    set(${out_var}_version "${${out_var}_version}" PARENT_SCOPE)
    set(${out_var}_bundle_version "${${out_var}_bundle_version}" PARENT_SCOPE)
    set(${out_var}_versioned_binary_dir "${${out_var}_versioned_binary_dir}" PARENT_SCOPE)
    set(${out_var}_versioned_header_dir "${${out_var}_versioned_header_dir}" PARENT_SCOPE)
    set(${out_var}_private_header_dir "${${out_var}_private_header_dir}" PARENT_SCOPE)
    set(${out_var}_private_module_header_dir "${${out_var}_private_module_header_dir}" PARENT_SCOPE)
endfunction()
