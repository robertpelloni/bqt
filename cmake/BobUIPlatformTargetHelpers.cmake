# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Defines the public BobUI::Platform target, which serves as a dependency for all internal BobUI target
# as well as user projects consuming BobUI.
function(bobui_internal_setup_public_platform_target)
    bobui_internal_get_platform_definition_include_dir(
        install_interface_definition_dir
        build_interface_definition_dir
    )

    ## BobUIPlatform Target:
    bobui_internal_add_platform_target(Platform)
    target_include_directories(Platform
        INTERFACE
        $<BUILD_INTERFACE:${build_interface_definition_dir}>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:${install_interface_definition_dir}>
        $<INSTALL_INTERFACE:${INSTALL_INCLUDEDIR}>
        )
    target_compile_definitions(Platform INTERFACE ${BOBUI_PLATFORM_DEFINITIONS})

    set_target_properties(Platform PROPERTIES
        _bobui_package_version "${PROJECT_VERSION}"
    )
    set_property(TARGET Platform
                 APPEND PROPERTY
                 EXPORT_PROPERTIES "_bobui_package_version")

    # When building on android we need to link against the logging library
    # in order to satisfy linker dependencies. Both of these libraries are part of
    # the NDK.
    if (ANDROID)
        if(BOBUI_FEATURE_android_16kb_pages)
            target_link_options(Platform INTERFACE "-Wl,-z,max-page-size=16384")
        endif()
        target_link_libraries(Platform INTERFACE log)
    endif()

    if (BOBUI_FEATURE_stdlib_libcpp)
        target_compile_options(Platform INTERFACE "$<$<COMPILE_LANGUAGE:CXX>:-stdlib=libc++>")
        set(libc_link_option "-stdlib=libc++")
        if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.18")
            set(libc_link_option "$<$<LINK_LANGUAGE:CXX>:-stdlib=libc++>")
        endif()
        target_link_options(Platform INTERFACE "${libc_link_option}")
    endif()
    if (BOBUI_FEATURE_no_direct_extern_access)
        target_compile_options(Platform INTERFACE "$<$<CXX_COMPILER_ID:GNU>:-mno-direct-extern-access>")
        target_compile_options(Platform INTERFACE "$<$<CXX_COMPILER_ID:Clang>:-fno-direct-access-external-data>")
    endif()

    bobui_set_msvc_cplusplus_options(Platform INTERFACE)

    # Propagate minimum C++ 17 via Platform to BobUI consumers (apps), after the global features
    # are computed.
    bobui_set_language_standards_interface_compile_features(Platform)

    # By default enable utf8 sources for both BobUI and BobUI consumers. Can be opted out.
    bobui_enable_utf8_sources(Platform)

    # By default enable unicode on WIN32 platforms for both BobUI and BobUI consumers. Can be opted out.
    bobui_internal_enable_unicode_defines(Platform)

    # Generate a pkgconfig for BobUI::Platform.
    bobui_internal_generate_pkg_config_file(Platform)

    bobui_internal_add_sbom(Platform
        SBOM_ENTITY_TYPE BOBUI_MODULE
        ATTRIBUTION_FILE_DIR_PATHS
            "${PROJECT_SOURCE_DIR}/cmake/3rdparty/extra-cmake-modules"
            "${PROJECT_SOURCE_DIR}/cmake/3rdparty/kwin"
    )
endfunction()

function(bobui_internal_get_platform_definition_include_dir install_interface build_interface)
    # Used by consumers of prefix builds via INSTALL_INTERFACE (relative path).
    set(${install_interface} "${INSTALL_MKSPECSDIR}/${BOBUI_QMAKE_TARGET_MKSPEC}" PARENT_SCOPE)

    # Used by bobuibase in prefix builds via BUILD_INTERFACE
    set(build_interface_base_dir
        "${CMAKE_CURRENT_LIST_DIR}/../mkspecs"
    )

    # Used by bobuibase and consumers in non-prefix builds via BUILD_INTERFACE
    if(NOT BOBUI_WILL_INSTALL)
        set(build_interface_base_dir
            "${BOBUI_BUILD_DIR}/${INSTALL_MKSPECSDIR}"
        )
    endif()

    get_filename_component(build_interface_dir
        "${build_interface_base_dir}/${BOBUI_QMAKE_TARGET_MKSPEC}"
        ABSOLUTE
    )
    set(${build_interface} "${build_interface_dir}" PARENT_SCOPE)
endfunction()
