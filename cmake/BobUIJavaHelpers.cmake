# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# This function can be used to compile java sources into a jar package.

function(bobui_internal_add_jar target)
    set(options)
    set(oneValueArgs OUTPUT_DIR)
    set(multiValueArgs INCLUDE_JARS SOURCES)
    cmake_parse_arguments(arg "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(javac_target_version "${BOBUI_ANDROID_JAVAC_TARGET}")
    if (NOT javac_target_version)
        set(javac_target_version "8")
    endif()

    set(javac_source_version "${BOBUI_ANDROID_JAVAC_SOURCE}")
    if (NOT javac_source_version)
        set(javac_source_version "8")
    endif()

    set(CMAKE_JAVA_COMPILE_FLAGS -source "${javac_source_version}" -target "${javac_target_version}"
        -Xlint:all -Xdoclint:all,-missing -classpath "${BOBUI_ANDROID_JAR}"
    )

    set(absolute_sources "")
    foreach(path IN LISTS arg_SOURCES)
        get_filename_component(absolute_path "${path}" ABSOLUTE)
        list(APPEND absolute_sources "${absolute_path}")
    endforeach()
    set_property(DIRECTORY APPEND PROPERTY _bobui_jar_sources "${absolute_sources}")

    add_jar(${target} SOURCES ${absolute_sources} ${ARGV})

    foreach(f IN LISTS arg_SOURCES)
        _bobui_internal_expose_source_file_to_ide(${target} "${f}")
    endforeach()

endfunction()
