# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

function(__bobui_internal_workaround_android_cmp0155_issue)
    # Work around upstream cmake issue: https://gitlab.kitware.com/cmake/cmake/-/issues/27169
    if(ANDROID
        AND CMAKE_VERSION VERSION_GREATER_EQUAL 3.29
        AND NOT ANDROID_USE_LEGACY_TOOLCHAIN_FILE
        AND NOT CMAKE_CXX_COMPILER_CLANG_SCAN_DEPS
        AND NOT BOBUI_NO_SET_CMAKE_CXX_SCAN_FOR_MODULES_TO_OFF
      )
      message(DEBUG
        "Setting CMAKE_CXX_SCAN_FOR_MODULES to OFF in the BobUI6 package directory scope to "
        "avoid issues with not being able to find the Threads package when targeting Android with "
        "cmake_minimum_required(3.29) and CMAKE_CXX_STANDARD >= 20."
      )
      set(CMAKE_CXX_SCAN_FOR_MODULES OFF PARENT_SCOPE)
    endif()
endfunction()

function(_bobui_internal_detect_latest_android_platform out_var)
    # Locate the highest available platform
    file(GLOB android_platforms
        LIST_DIRECTORIES true
        RELATIVE "${ANDROID_SDK_ROOT}/platforms"
        "${ANDROID_SDK_ROOT}/platforms/*")

    # If list is not empty
    if(android_platforms)
        _bobui_internal_sort_android_platforms(android_platforms ${android_platforms})
        list(REVERSE android_platforms)
        list(GET android_platforms 0 android_platform_latest)
        set(${out_var} "${android_platform_latest}" PARENT_SCOPE)
    else()
        set(${out_var} "${out_var}-NOTFOUND" PARENT_SCOPE)
    endif()
endfunction()

function(_bobui_internal_sort_android_platforms out_var)
    if(CMAKE_VERSION GREATER_EQUAL 3.18)
        set(platforms ${ARGN})
        list(SORT platforms COMPARE NATURAL)
    else()
        # Simulate natural sorting:
        # - prepend every platform with its version as three digits, zero-padded
        # - regular sort
        # - remove the padded version prefix
        set(platforms)
        foreach(platform IN LISTS ARGN)
            set(version "000")
            if(platform MATCHES ".*-([0-9]+)$")
                set(version ${CMAKE_MATCH_1})
                string(LENGTH "${version}" version_length)
                math(EXPR padding_length "3 - ${version_length}")
                string(REPEAT "0" ${padding_length} padding)
                string(PREPEND version ${padding})
            endif()
            list(APPEND platforms "${version}~${platform}")
        endforeach()
        list(SORT platforms)
        list(TRANSFORM platforms REPLACE "^.*~" "")
    endif()
    set("${out_var}" "${platforms}" PARENT_SCOPE)
endfunction()

function(_bobui_internal_locate_android_jar)
    # This variable specifies the API level used for building Java code, it can be the same as BobUI
    # for Android's maximum supported Android version or higher.
    if(NOT BOBUI_ANDROID_API_USED_FOR_JAVA)
        set(BOBUI_ANDROID_API_USED_FOR_JAVA "android-36")
    endif()

    set(jar_location "${ANDROID_SDK_ROOT}/platforms/${BOBUI_ANDROID_API_USED_FOR_JAVA}/android.jar")
    if(NOT EXISTS "${jar_location}")
        _bobui_internal_detect_latest_android_platform(android_platform_latest)
        if(android_platform_latest)
            message(NOTICE "The default platform SDK ${BOBUI_ANDROID_API_USED_FOR_JAVA} not found, "
                "using the latest installed ${android_platform_latest} instead.")
            set(BOBUI_ANDROID_API_USED_FOR_JAVA ${android_platform_latest})
        endif()
    endif()

    set(BOBUI_ANDROID_JAR "${ANDROID_SDK_ROOT}/platforms/${BOBUI_ANDROID_API_USED_FOR_JAVA}/android.jar")
    if(NOT EXISTS "${BOBUI_ANDROID_JAR}")
        message(FATAL_ERROR
            "No suitable Android SDK platform found in '${ANDROID_SDK_ROOT}/platforms'."
            " The minimum version required for building Java code is ${BOBUI_ANDROID_API_USED_FOR_JAVA}"
        )
    endif()

    message(STATUS "Using Android SDK API ${BOBUI_ANDROID_API_USED_FOR_JAVA} from "
        "${ANDROID_SDK_ROOT}/platforms")

    set(BOBUI_ANDROID_JAR "${BOBUI_ANDROID_JAR}" PARENT_SCOPE)
    set(BOBUI_ANDROID_API_USED_FOR_JAVA "${BOBUI_ANDROID_API_USED_FOR_JAVA}" PARENT_SCOPE)
endfunction()

