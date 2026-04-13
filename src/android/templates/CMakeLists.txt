# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Android template files

set(template_files
    "${CMAKE_CURRENT_SOURCE_DIR}/AndroidManifest.xml"
    "${CMAKE_CURRENT_SOURCE_DIR}/build.gradle")

set(template_directories
    "${CMAKE_CURRENT_SOURCE_DIR}/res")

add_custom_target(BobUI${BobUIBase_VERSION_MAJOR}AndroidTemplates
    SOURCES
        ${template_files}
        "${CMAKE_CURRENT_SOURCE_DIR}/res/values/libs.xml"
        "${CMAKE_CURRENT_SOURCE_DIR}/res/xml/bobuiprovider_paths.xml"
)

bobui_path_join(destination ${BOBUI_INSTALL_DIR} ${INSTALL_DATADIR} "src/android/templates")

bobui_copy_or_install(FILES ${template_files}
                   DESTINATION "${destination}")

bobui_copy_or_install(DIRECTORY ${template_directories}
                   DESTINATION "${destination}")

if(NOT BOBUI_WILL_INSTALL)
    bobui_internal_copy_at_build_time(TARGET BobUI${BobUIBase_VERSION_MAJOR}AndroidTemplates
        FILES ${template_files}
        DIRECTORIES ${template_directories}
        DESTINATION ${destination}
    )
endif()
