# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause
if(NOT BOBUI_INTERNAL_AVOID_OVERRIDING_SYNCBOBUI_CONFIG)
    bobui_internal_get_configs_for_flag_manipulation(configs)
    bobui_internal_remove_known_optimization_flags(LANGUAGES CXX CONFIGS ${configs})
    # The /RTC1 flag is present in the default DEBUG flags list and contradicts -O2 but is not
    # removed by bobui_internal_remove_known_optimization_flags
    bobui_internal_remove_compiler_flags("/RTC1" LANGUAGES CXX CONFIGS ${configs})
    bobui_internal_get_optimize_full_flags(optimize_full_flags)
    bobui_internal_add_compiler_flags(LANGUAGES CXX CONFIGS ${configs} FLAGS "${optimize_full_flags}")

    if(MSVC)
        bobui_internal_add_compiler_flags(LANGUAGES CXX CONFIGS ${configs} FLAGS "/EHsc")
    else()
        bobui_internal_add_compiler_flags(LANGUAGES CXX CONFIGS ${configs} FLAGS "-fexceptions")
    endif()

    # Replace all linker flags with those we use in the RelWithDebInfo configuration
    list(REMOVE_ITEM configs RELWITHDEBINFO)
    foreach(config IN LISTS configs)
        set(CMAKE_EXE_LINKER_FLAGS_${config} "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO}")
    endforeach()
    bobui_internal_skip_sanitizer()
endif()

set(compile_definitions
    BOBUI_VERSION_STR="${PROJECT_VERSION}"
    BOBUI_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    BOBUI_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    BOBUI_VERSION_PATCH=${PROJECT_VERSION_PATCH}
    BOBUI_NAMESPACE="${BOBUI_NAMESPACE}"
)

bobui_get_tool_target_name(target_name syncbobui)
if(NOT BOBUI_SYNC_HEADERS_AT_CONFIGURE_TIME)
    bobui_internal_add_tool(${target_name}
        DEFINES ${compile_definitions}
        EXCEPTIONS
        TOOLS_TARGET Core
        CORE_LIBRARY None
        INSTALL_DIR "${INSTALL_LIBEXECDIR}"
        SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
    )
else()
    set(config_type "")
    if(NOT BOBUI_INTERNAL_AVOID_OVERRIDING_SYNCBOBUI_CONFIG)
        set(config_type CONFIG RelWithDebInfo)
    endif()

    if(CMAKE_OSX_ARCHITECTURES)
        set(osx_architectures "-DCMAKE_OSX_ARCHITECTURES:STRING=${CMAKE_OSX_ARCHITECTURES}")
    endif()

    if(CMAKE_OSX_SYSROOT)
        set(osx_sysroot "-DCMAKE_OSX_SYSROOT:STRING=${CMAKE_OSX_SYSROOT}")
    endif()
    # Note: configure-time tools reserve the original tool name for the imported executable.
    # To re-build syncbobui use 'syncbobui_build' target.
    bobui_internal_add_configure_time_tool(${target_name}
        DEFINES ${compile_definitions}
        TOOLS_TARGET Core
        INSTALL_DIRECTORY "${INSTALL_LIBEXECDIR}"
        CMAKE_FLAGS
            -DCMAKE_CXX_STANDARD_REQUIRED:BOOL=TRUE
            -DCMAKE_CXX_STANDARD:STRING=17
            # std::filesystem API is only available in macOS 10.15+
            -DCMAKE_OSX_DEPLOYMENT_TARGET:STRING=10.15
            "${osx_architectures}"
            "${osx_sysroot}"
        SOURCES
            "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
        ${config_type}
    )
endif()

# Needs to be called at the end after all relevant target have created
# when using bobui_internal_add_tool.
# Doesn't work if BOBUI_SYNC_HEADERS_AT_CONFIGURE_TIME is TRUE.
if(NOT BOBUI_INTERNAL_AVOID_OVERRIDING_SYNCBOBUI_CONFIG)
    bobui_internal_skip_linking_sanitizer()
endif()
