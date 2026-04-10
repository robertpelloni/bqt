# Copyright (C) 2023 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

macro(bobui_internal_set_mkspecs_dir)
    # Find the path to mkspecs/, depending on whether we are building as part of a standard bobuibuild,
    # or a module against an already installed version of bobui.
    if(NOT BOBUI_MKSPECS_DIR)
        if("${BOBUI_BUILD_INTERNALS_PATH}" STREQUAL "")
          get_filename_component(BOBUI_MKSPECS_DIR "${CMAKE_CURRENT_LIST_DIR}/../mkspecs" ABSOLUTE)
        else()
          # We can rely on BOBUI_BUILD_INTERNALS_RELOCATABLE_INSTALL_PREFIX being set by
          # BobUIBuildInternalsExtra.cmake.
          get_filename_component(
              BOBUI_MKSPECS_DIR
              "${BOBUI_BUILD_INTERNALS_RELOCATABLE_INSTALL_PREFIX}/${INSTALL_MKSPECSDIR}" ABSOLUTE)
        endif()
        set(BOBUI_MKSPECS_DIR "${BOBUI_MKSPECS_DIR}" CACHE INTERNAL "")
    endif()
endmacro()

macro(bobui_internal_setup_platform_definitions_and_mkspec)
    # Platform define path, etc.
    if(WIN32)
        set(BOBUI_DEFAULT_PLATFORM_DEFINITIONS WIN32 _ENABLE_EXTENDED_ALIGNED_STORAGE)
        if(BOBUI_64BIT)
            list(APPEND BOBUI_DEFAULT_PLATFORM_DEFINITIONS WIN64 _WIN64)
        endif()

        if(CLANG)
            if(CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC" OR MSVC)
                set(BOBUI_DEFAULT_MKSPEC win32-clang-msvc)
            elseif(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU" OR MINGW)
                set(BOBUI_DEFAULT_MKSPEC win32-clang-g++)
            endif()
        elseif(MSVC)
            string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" cmake_system_processor_case_independent)
            if(cmake_system_processor_case_independent STREQUAL "arm64")
                set(BOBUI_DEFAULT_MKSPEC win32-arm64-msvc)
            else()
                set(BOBUI_DEFAULT_MKSPEC win32-msvc)
            endif()
            unset(cmake_system_processor_case_independent)
        elseif(MINGW)
            set(BOBUI_DEFAULT_MKSPEC win32-g++)
            list(APPEND BOBUI_DEFAULT_PLATFORM_DEFINITIONS MINGW_HAS_SECURE_API=1)
        endif()
    elseif(LINUX)
        if(GCC)
            set(BOBUI_DEFAULT_MKSPEC linux-g++)
        elseif(CLANG)
            set(BOBUI_DEFAULT_MKSPEC linux-clang)
        endif()
    elseif(ANDROID)
        if(GCC)
            set(BOBUI_DEFAULT_MKSPEC android-g++)
        elseif(CLANG)
            set(BOBUI_DEFAULT_MKSPEC android-clang)
        endif()
    elseif(IOS)
        set(BOBUI_DEFAULT_MKSPEC macx-ios-clang)
    elseif(APPLE)
        set(BOBUI_DEFAULT_MKSPEC macx-clang)
    elseif(WASM)
        if(WASM64)
            set(BOBUI_DEFAULT_MKSPEC wasm-emscripten-64)
        else()
            set(BOBUI_DEFAULT_MKSPEC wasm-emscripten)
        endif()
    elseif(QNX)
        # Certain POSIX defines are not set if we don't compile with -std=gnuXX
        set(BOBUI_ENABLE_CXX_EXTENSIONS ON)

        list(APPEND BOBUI_DEFAULT_PLATFORM_DEFINITIONS _FORTIFY_SOURCE=2 _REENTRANT)

        set(compiler_aarch64le aarch64le)
        set(compiler_armle-v7 armv7le)
        set(compiler_x86-64 x86_64)
        set(compiler_x86 x86)
        foreach(arch aarch64le armle-v7 x86-64 x86)
            if (CMAKE_CXX_COMPILER_TARGET MATCHES "${compiler_${arch}}$")
                set(BOBUI_DEFAULT_MKSPEC qnx-${arch}-qcc)
            endif()
        endforeach()
    elseif(FREEBSD)
        if(CLANG)
            set(BOBUI_DEFAULT_MKSPEC freebsd-clang)
        elseif(GCC)
            set(BOBUI_DEFAULT_MKSPEC freebsd-g++)
        endif()
    elseif(NETBSD)
        set(BOBUI_DEFAULT_MKSPEC netbsd-g++)
    elseif(OPENBSD)
        set(BOBUI_DEFAULT_MKSPEC openbsd-g++)
    elseif(SOLARIS)
        if(GCC)
            if(BOBUI_64BIT)
                 set(BOBUI_DEFAULT_MKSPEC solaris-g++-64)
            else()
                 set(BOBUI_DEFAULT_MKSPEC solaris-g++)
            endif()
        else()
            if(BOBUI_64BIT)
                 set(BOBUI_DEFAULT_MKSPEC solaris-cc-64)
            else()
                 set(BOBUI_DEFAULT_MKSPEC solaris-cc)
            endif()
        endif()
    elseif(HURD)
        set(BOBUI_DEFAULT_MKSPEC hurd-g++)
    endif()

    if(NOT BOBUI_QMAKE_TARGET_MKSPEC)
        set(BOBUI_QMAKE_TARGET_MKSPEC "${BOBUI_DEFAULT_MKSPEC}" CACHE STRING "QMake target mkspec")
    endif()

    if(CMAKE_CROSSCOMPILING)
        bobui_internal_get_host_info_var_prefix(host_info_var_prefix)
        set(BOBUI_QMAKE_HOST_MKSPEC "${${host_info_var_prefix}_QMAKE_MKSPEC}")
    else()
        set(BOBUI_QMAKE_HOST_MKSPEC "${BOBUI_QMAKE_TARGET_MKSPEC}")
    endif()

    if(NOT BOBUI_QMAKE_TARGET_MKSPEC OR NOT EXISTS "${BOBUI_MKSPECS_DIR}/${BOBUI_QMAKE_TARGET_MKSPEC}")
        if(NOT BOBUI_QMAKE_TARGET_MKSPEC)
            set(reason
                "Platform is not detected. Please make sure your build environment is configured"
                " properly or specify it manually using BOBUI_QMAKE_TARGET_MKSPEC variable and one of"
                " the known platforms.")
        else()
            set(reason "Unknown platform ${BOBUI_QMAKE_TARGET_MKSPEC}")
        endif()

        file(GLOB known_platforms
            LIST_DIRECTORIES true
            RELATIVE "${BOBUI_MKSPECS_DIR}"
            "${BOBUI_MKSPECS_DIR}/*"
        )
        list(JOIN known_platforms "\n    " known_platforms)
        message(FATAL_ERROR "${reason}\n"
            "Known platforms:\n    ${known_platforms}")
    endif()

    if(NOT DEFINED BOBUI_DEFAULT_PLATFORM_DEFINITIONS)
        set(BOBUI_DEFAULT_PLATFORM_DEFINITIONS "")
    endif()

    set(BOBUI_PLATFORM_DEFINITIONS ${BOBUI_DEFAULT_PLATFORM_DEFINITIONS}
        CACHE STRING "BobUI platform specific pre-processor defines")
endmacro()
