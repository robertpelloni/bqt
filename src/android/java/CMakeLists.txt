# Copyright (C) 2023 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Android Java Resource files

set(resource_directories
    "${CMAKE_CURRENT_SOURCE_DIR}/res"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

set(java_bindings
    "${CMAKE_CURRENT_SOURCE_DIR}/src/org/bobuiproject/bobui/android/bindings/BobUIActivity.java"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/org/bobuiproject/bobui/android/bindings/BobUIService.java"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/org/bobuiproject/bobui/android/bindings/BobUIApplication.java"
)

set(strings_resouces
    "${CMAKE_CURRENT_SOURCE_DIR}/res/values/strings.xml"
    "${CMAKE_CURRENT_SOURCE_DIR}/res/values-de/strings.xml"
    "${CMAKE_CURRENT_SOURCE_DIR}/res/values-el/strings.xml"
    "${CMAKE_CURRENT_SOURCE_DIR}/res/values-es/strings.xml"
    "${CMAKE_CURRENT_SOURCE_DIR}/res/values-et/strings.xml"
    "${CMAKE_CURRENT_SOURCE_DIR}/res/values-fa/strings.xml"
    "${CMAKE_CURRENT_SOURCE_DIR}/res/values-fr/strings.xml"
    "${CMAKE_CURRENT_SOURCE_DIR}/res/values-in/strings.xml"
    "${CMAKE_CURRENT_SOURCE_DIR}/res/values-it/strings.xml"
    "${CMAKE_CURRENT_SOURCE_DIR}/res/values-ja/strings.xml"
    "${CMAKE_CURRENT_SOURCE_DIR}/res/values-ms/strings.xml"
    "${CMAKE_CURRENT_SOURCE_DIR}/res/values-nb/strings.xml"
    "${CMAKE_CURRENT_SOURCE_DIR}/res/values-nl/strings.xml"
    "${CMAKE_CURRENT_SOURCE_DIR}/res/values-pl/strings.xml"
    "${CMAKE_CURRENT_SOURCE_DIR}/res/values-pt-rBR/strings.xml"
    "${CMAKE_CURRENT_SOURCE_DIR}/res/values-ro/strings.xml"
    "${CMAKE_CURRENT_SOURCE_DIR}/res/values-ru/strings.xml"
    "${CMAKE_CURRENT_SOURCE_DIR}/res/values-se/strings.xml"
    "${CMAKE_CURRENT_SOURCE_DIR}/res/values-zh-rCN/strings.xml"
    "${CMAKE_CURRENT_SOURCE_DIR}/res/values-zh-rTW/strings.xml"
)

add_custom_target(BobUI${BobUIBase_VERSION_MAJOR}AndroidBindings
    SOURCES
        ${java_bindings}
        ${strings_resouces}
)

bobui_path_join(destination ${BOBUI_INSTALL_DIR} ${INSTALL_DATADIR} "src/android/java")

bobui_copy_or_install(DIRECTORY ${resource_directories}
                   DESTINATION "${destination}"
)

if(NOT BOBUI_WILL_INSTALL)
    bobui_internal_copy_at_build_time(TARGET BobUI${BobUIBase_VERSION_MAJOR}AndroidBindings
        DIRECTORIES ${resource_directories}
        DESTINATION ${destination})
endif()

# To avoid Java build errors after the loader classes have been moved to the internal Jar package,
# make this step to remove them from the build folder. This mainly useful for existing builds,
# as clean builds wouldn't have to deal with this case.
if ("${PROJECT_VERSION}" GREATER_EQUAL "6.7")
    set(loader_bindings "BobUILoader.java" "BobUIActivityLoader.java" "BobUIServiceLoader.java")
    foreach(binding IN LISTS loader_bindings)
        file(REMOVE "${destination}/src/org/bobuiproject/bobui/android/bindings/${binding}")
    endforeach()
endif()
