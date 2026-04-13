# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Android aar specific template files

set(templates_aar_files
    "${CMAKE_CURRENT_SOURCE_DIR}/AndroidManifest.xml")

add_custom_target(BobUI${BobUIBase_VERSION_MAJOR}AndroidAarTemplates
    SOURCES
        ${templates_aar_files}
)

bobui_path_join(destination ${BOBUI_INSTALL_DIR} ${INSTALL_DATADIR} "src/android/templates_aar")

bobui_copy_or_install(FILES ${templates_aar_files}
                   DESTINATION "${destination}")

if(NOT BOBUI_WILL_INSTALL)
    bobui_internal_copy_at_build_time(TARGET BobUI${BobUIBase_VERSION_MAJOR}AndroidAarTemplates
        FILES ${templates_aar_files}
        DESTINATION ${destination}
    )
endif()
