# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause
set(gradle_programs
    "${CMAKE_CURRENT_SOURCE_DIR}/gradlew"
)
set(gradle_files
    "${CMAKE_CURRENT_SOURCE_DIR}/gradlew.bat"
    "${CMAKE_CURRENT_SOURCE_DIR}/gradle.properties"
)
set(gradle_wrapper
    "${CMAKE_CURRENT_SOURCE_DIR}/gradle"
)
set(gradle_wrapper_files
    "${CMAKE_CURRENT_SOURCE_DIR}/gradle/wrapper/gradle-wrapper.properties"
    "${CMAKE_CURRENT_SOURCE_DIR}/gradle/wrapper/gradle-wrapper.jar"
)

add_custom_target(BobUI${BobUIBase_VERSION_MAJOR}GradleScripts
    SOURCES
        ${gradle_programs}
        ${gradle_files}
        ${gradle_wrapper_files}
)

bobui_path_join(destination ${BOBUI_INSTALL_DIR} ${INSTALL_DATADIR} "src/3rdparty/gradle")

bobui_copy_or_install(
    PROGRAMS
        ${gradle_programs}
    DESTINATION
        "${destination}"
)

bobui_copy_or_install(
    FILES
        ${gradle_files}
    DESTINATION
        "${destination}"
)

bobui_copy_or_install(
    DIRECTORY
        ${gradle_wrapper}
    DESTINATION
        "${destination}"
)

if(NOT BOBUI_WILL_INSTALL)
    bobui_internal_copy_at_build_time(TARGET BobUI${BobUIBase_VERSION_MAJOR}GradleScripts
        FILES ${gradle_programs} ${gradle_files}
        DIRECTORIES ${gradle_wrapper}
        DESTINATION ${destination}
    )
endif()
