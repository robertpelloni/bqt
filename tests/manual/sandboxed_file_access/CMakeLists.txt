# Copyright (C) 2025 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause
if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_manual_sandboxed_file_access LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_standard_project_setup()

bobui_add_executable(tst_manual_sandboxed_file_access
    tst_sandboxed_file_access.cpp
)

target_link_libraries(tst_manual_sandboxed_file_access PRIVATE
    BobUI::CorePrivate
    BobUI::Widgets
    BobUI::Test
)

enable_language(OBJCXX)
set_source_files_properties(tst_sandboxed_file_access.cpp PROPERTIES LANGUAGE OBJCXX)

if(MACOS)
    target_sources(tst_manual_sandboxed_file_access PRIVATE app.entitlements)
    set_target_properties(tst_manual_sandboxed_file_access PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER "io.bobui.dev.tst-manual-sandboxed-file-access"
        XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS "${CMAKE_CURRENT_SOURCE_DIR}/app.entitlements"
        XCODE_ATTRIBUTE_COPY_PHASE_STRIP FALSE
    )
    if(NOT CMAKE_GENERATOR STREQUAL "Xcode")
        set_target_properties(tst_manual_sandboxed_file_access PROPERTIES
            RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/app.entitlements"
        )
    endif()

    set(platform_plugin "${BOBUI6_INSTALL_PREFIX}/${BOBUI6_INSTALL_PLUGINS}/platforms/libqcocoa.dylib")
    target_sources(tst_manual_sandboxed_file_access PRIVATE ${platform_plugin})
    set_source_files_properties(${platform_plugin}
        PROPERTIES
        MACOSX_PACKAGE_LOCATION PlugIns/platforms
    )

    target_compile_definitions(tst_manual_sandboxed_file_access PRIVATE
        BOBUIEST_THROW_ON_FAIL
        BOBUIEST_THROW_ON_SKIP
    )
endif()

if(IOS)
    set(plist_path "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.ios")
else()
    set(plist_path "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.macos")
endif()

set_target_properties(tst_manual_sandboxed_file_access
    PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${plist_path}")

install(TARGETS tst_manual_sandboxed_file_access
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET tst_manual_sandboxed_file_access
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
