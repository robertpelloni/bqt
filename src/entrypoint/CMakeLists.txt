# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if (NOT (WIN32 OR UIKIT))
    return()
endif()

# The EntryPointPrivate package consists of two targets: one for CMake consumption,
# and one internal that produces the static library. Together these form the
# entrypoint module in qmake terms. This split allows us to inject library
# dependencies that need to go _before_ the static library, to work around
# CMake's lack of whole archive.

# ---- Set up an intermediate imported library for libmingw32.a ----

set(export_name_prefix "${INSTALL_CMAKE_NAMESPACE}EntryPointPrivate")
bobui_path_join(config_install_dir ${BOBUI_CONFIG_INSTALL_DIR} ${export_name_prefix})

set(extra_cmake_includes_arg)
if(MINGW)
    # The mingw32 library needs to come before the entry-point library in the linker line, so that
    # the static linker will pick up the WinMain symbol from the entry-point library.  In order to
    # achieve that reliably, we create an imported library EntryPointMinGW32 that represents
    # libmingw32.a and add a link dependency to EntryPointImplementation.  The resulting dependency
    # chain looks like this: EntryPointPrivate -> EntryPointMinGW32 -> EntryPointImplementation

    set(mingw32target_config_file "${INSTALL_CMAKE_NAMESPACE}EntryPointMinGW32Target.cmake")
    configure_file("EntryPointMinGW32Target.cmake.in" "${mingw32target_config_file}" @ONLY)
    bobui_copy_or_install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${mingw32target_config_file}"
        DESTINATION "${config_install_dir}")
    # In prefix builds we also need to copy the file into the build config directory, so that the
    # build-dir BobUI6Config.cmake finds the files when building other repos in a top-level build.
    if(BOBUI_WILL_INSTALL)
        get_filename_component(absolute_config_install_dir "${config_install_dir}" ABSOLUTE
            BASE_DIR "${BOBUI_BUILD_DIR}")
        file(COPY "${CMAKE_CURRENT_BINARY_DIR}/${mingw32target_config_file}"
             DESTINATION "${absolute_config_install_dir}")
    endif()
    include("${CMAKE_CURRENT_BINARY_DIR}/${mingw32target_config_file}")
    set(extra_cmake_includes_arg EXTRA_CMAKE_INCLUDES "${mingw32target_config_file}")
endif()

# ---- The header-only target produces the actual module ----
bobui_internal_add_module(EntryPointPrivate
    HEADER_MODULE
    INTERNAL_MODULE
    NO_SYNC_BOBUI
    NO_MODULE_HEADERS
    NO_PRIVATE_MODULE
    NO_ADDITIONAL_TARGET_INFO
    ${extra_cmake_includes_arg}
    BOBUI_LICENSE_ID BOBUI_COMMERCIAL_OR_BSD3
)

set(export_targets EntryPointPrivate)
# We don't need any include paths or default module defines
set_target_properties(EntryPointPrivate PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ""
    INTERFACE_COMPILE_DEFINITIONS ""
)

if(WIN32)
    # Not all platforms require the static library
    set(using_entrypoint_library "yes")
endif()

if(using_entrypoint_library)
    # ---- While the static library target does the work ----
    bobui_internal_add_cmake_library(EntryPointImplementation STATIC
        INCLUDE_DIRECTORIES
           $<TARGET_PROPERTY:BobUI::Core,INTERFACE_INCLUDE_DIRECTORIES>
    )

    list(APPEND export_targets EntryPointImplementation)

    set_target_properties(EntryPointImplementation PROPERTIES
        OUTPUT_NAME "${INSTALL_CMAKE_NAMESPACE}EntryPoint${BOBUI_LIBINFIX}"
        ARCHIVE_OUTPUT_DIRECTORY "${BOBUI_BUILD_DIR}/${INSTALL_LIBDIR}"
    )

    bobui_handle_multi_config_output_dirs(EntryPointImplementation)
    bobui_internal_add_target_aliases(EntryPointImplementation)
    bobui_set_common_target_properties(EntryPointImplementation)
endif()

# ---- Now we're ready to set up the platform specifics ----

if(WIN32)
    bobui_internal_extend_target(EntryPointImplementation
        SOURCES bobuientrypoint_win.cpp
        LIBRARIES shell32
    )

    if(MSVC)
        # Store debug information inside the static lib
        bobui_internal_replace_compiler_flags(
            "/Zi" "/Z7"
            CONFIGS DEBUG RELWITHDEBINFO)
    endif()

    if(MINGW)
        # Link against EntryPointImplementation via EntryPointMinGW32
        target_link_libraries(EntryPointPrivate INTERFACE EntryPointMinGW32)
        set_property(TARGET EntryPointPrivate
            APPEND PROPERTY INTERFACE_BOBUI_MODULE_LDFLAGS "-lmingw32"
        )

        target_compile_definitions(EntryPointPrivate INTERFACE BOBUI_NEEDS_QMAIN)
        bobui_internal_extend_target(EntryPointImplementation DEFINES BOBUI_NEEDS_QMAIN)
    endif()

    bobui_internal_add_sync_header_dependencies(EntryPointImplementation Core)
endif()

if(UIKIT)
    set_target_properties(EntryPointPrivate PROPERTIES
        INTERFACE_LINK_OPTIONS "-Wl,-e,_bobui_main_wrapper"
    )
    set_property(TARGET EntryPointPrivate
        APPEND PROPERTY INTERFACE_BOBUI_MODULE_LDFLAGS "-Wl,-e,_bobui_main_wrapper"
    )
endif()

# ---- Finally, make sure the static library can be consumed by clients -----

if(using_entrypoint_library)
    target_link_libraries(EntryPointPrivate INTERFACE BobUI6::EntryPointImplementation)

    bobui_internal_get_target_property(entrypoint_implementation_ldflags
        EntryPointImplementation BOBUI_MODULE_LDFLAGS)

    set_target_properties(EntryPointPrivate PROPERTIES
        INTERFACE_BOBUI_MODULE_PRI_EXTRA_CONTENT "
BOBUI.entrypoint_implementation.name = BobUIEntryPointImplementation
BOBUI.entrypoint_implementation.module = BobUI6EntryPoint${BOBUI_LIBINFIX}
BOBUI.entrypoint_implementation.ldflags = ${entrypoint_implementation_ldflags}
BOBUI.entrypoint_implementation.libs = $$BOBUI_MODULE_LIB_BASE
BOBUI.entrypoint_implementation.module_config = staticlib v2 internal_module
"
        INTERFACE_BOBUI_MODULE_DEPENDS "entrypoint_implementation"
    )

    set(export_name "${INSTALL_CMAKE_NAMESPACE}EntryPointPrivateTargets")
    bobui_install(TARGETS EntryPointImplementation EXPORT ${export_name})
    bobui_generate_prl_file(EntryPointImplementation "${INSTALL_LIBDIR}")
endif()

bobui_internal_export_additional_targets_file(
    TARGETS ${export_targets}
    EXPORT_NAME_PREFIX ${export_name_prefix}
    CONFIG_INSTALL_DIR "${config_install_dir}")
