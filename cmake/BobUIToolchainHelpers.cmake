# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Create a CMake toolchain file for convenient configuration of both internal BobUI builds
# as well as CMake application projects.
# Expects various global variables to be set.
function(bobui_internal_create_toolchain_file)
    if(CMAKE_TOOLCHAIN_FILE)
        file(TO_CMAKE_PATH "${CMAKE_TOOLCHAIN_FILE}" __bobui_chainload_toolchain_file)
        set(init_original_toolchain_file
            "
set(__bobui_initially_configured_toolchain_file \"${__bobui_chainload_toolchain_file}\")
set(__bobui_chainload_toolchain_file \"\${__bobui_initially_configured_toolchain_file}\")
")
    endif()

    if(BOBUI_USE_VCPKG)
        set(init_vcpkg "set(__bobui_initially_configured_use_vcpkg TRUE)")
        if(VCPKG_CHAINLOAD_TOOLCHAIN_FILE)
            file(TO_CMAKE_PATH "${VCPKG_CHAINLOAD_TOOLCHAIN_FILE}"
                initial_vcpkg_chainload_toolchain_file)
            get_filename_component(initial_vcpkg_chainload_toolchain_file
                "${initial_vcpkg_chainload_toolchain_file}" REALPATH)
            list(APPEND init_vcpkg
                "set(__bobui_initially_configured_vcpkg_chainload_toolchain_file \
    \"${initial_vcpkg_chainload_toolchain_file}\")")
        endif()

        if(VCPKG_TARGET_TRIPLET)
            list(APPEND init_vcpkg
                "set(__bobui_initially_configured_vcpkg_target_triplet \"${VCPKG_TARGET_TRIPLET}\")")
        endif()
    else()
        set(init_vcpkg "")
    endif()

    if(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND CMAKE_CROSSCOMPILING)
        list(APPEND init_platform
            "string(TOUPPER \"${CMAKE_SYSTEM_PROCESSOR}\" _bobui_orig_target_system_processor_upper)"
            "string(TOUPPER \"\${CMAKE_HOST_SYSTEM_PROCESSOR}\" _bobui_host_system_processor_upper)"
            "if(NOT _bobui_orig_target_system_processor_upper STREQUAL _bobui_host_system_processor_upper"
            "    OR NOT CMAKE_HOST_SYSTEM_VERSION MATCHES \"^${CMAKE_SYSTEM_VERSION}(\\\\..+|$)\""
            "    OR BOBUI_FORCE_CROSSCOMPILING)"
            ""
            "    set(CMAKE_SYSTEM_NAME \"${CMAKE_SYSTEM_NAME}\" CACHE STRING \"\")"
            "    set(CMAKE_SYSTEM_VERSION \"${CMAKE_SYSTEM_VERSION}\" CACHE STRING \"\")"
            "    set(CMAKE_SYSTEM_PROCESSOR \"${CMAKE_SYSTEM_PROCESSOR}\" CACHE STRING \"\")"
            "else()"
            "   set(BOBUI_REQUIRE_HOST_PATH_CHECK FALSE)"
            "endif()"
            "unset(_bobui_host_system_processor_upper)"
            "unset(_bobui_orig_host_system_processor_upper)"
        )
    endif()

    if(BOBUI_QMAKE_TARGET_MKSPEC)
        list(APPEND init_platform
            "if(NOT BOBUI_QMAKE_TARGET_MKSPEC)"
            "    set(BOBUI_QMAKE_TARGET_MKSPEC ${BOBUI_QMAKE_TARGET_MKSPEC} CACHE STRING \"\")"
            "endif()"
        )
    endif()

    if("${BOBUI_QMAKE_TARGET_MKSPEC}" STREQUAL "linux-g++-32" AND NOT BOBUI_NO_AUTO_DETECT_LINUX_X86)
        set(__bobui_toolchain_common_flags_init "-m32")

        if(NOT BOBUI_NO_OVERRIDE_LANG_FLAGS_INIT)
            list(APPEND init_platform
                "if(NOT BOBUI_NO_OVERRIDE_LANG_FLAGS_INIT)")

            list(APPEND init_platform
                "    set(__bobui_toolchain_common_flags_init \"-m32\")")
            list(APPEND init_platform
                "    set(CMAKE_C_FLAGS_INIT \"\${__bobui_toolchain_common_flags_init}\")")
            list(APPEND init_platform
                "    set(CMAKE_CXX_FLAGS_INIT \"\${__bobui_toolchain_common_flags_init}\")")
            list(APPEND init_platform
                "    set(CMAKE_ASM_FLAGS_INIT \"\${__bobui_toolchain_common_flags_init}\")")

            list(APPEND init_platform "endif()")
        endif()

        # Ubuntu-specific paths are used below.
        # See comments of bobui_auto_detect_linux_x86() for details.
        if(NOT BOBUI_NO_OVERRIDE_CMAKE_IGNORE_PATH)
            list(APPEND init_platform
                "if(NOT BOBUI_NO_OVERRIDE_CMAKE_IGNORE_PATH)")

            get_property(linux_x86_ignore_path GLOBAL PROPERTY _bobui_internal_linux_x86_ignore_path)

            string(REPLACE ";" "LITERAL_SEMICOLON"
                linux_x86_ignore_path "${linux_x86_ignore_path}")

            list(APPEND init_platform
                "    set(CMAKE_IGNORE_PATH \"${linux_x86_ignore_path}\")")

            list(APPEND init_platform "endif()")
        endif()

        if(NOT BOBUI_NO_OVERRIDE_PKG_CONFIG_LIBDIR)
            list(APPEND init_platform
                "if(NOT BOBUI_NO_OVERRIDE_PKG_CONFIG_LIBDIR)")

            get_property(pc_config_libdir GLOBAL PROPERTY _bobui_internal_linux_x86_pc_config_libdir)

            list(APPEND init_platform
                "    set(ENV{PKG_CONFIG_LIBDIR} \"${pc_config_libdir}\")")
            list(APPEND init_platform
                "    set(ENV{PKG_CONFIG_DIR} \"\")")

            list(APPEND init_platform "endif()")
        endif()
    endif()

    # By default we don't want to allow mixing compilers for building different repositories, so we
    # embed the initially chosen compilers into the toolchain.
    # This is because on Windows compilers aren't easily mixed.
    # We want to avoid that bobuibase is built using cl.exe for example, and then for another repo
    # gcc is picked up from %PATH%.
    # The same goes when using a custom compiler on other platforms, such as ICC.
    #
    # There are a few exceptions though.
    #
    # When crosscompiling using Boot2BobUI, the environment setup shell script sets up the CXX env var,
    # which is used by CMake to determine the initial compiler that should be used.
    # Unfortunately, the CXX env var contains not only the compiler name, but also a few required
    # arch-specific compiler flags. This means that when building bobuisvg, if the BobUI created toolchain
    # file sets the CMAKE_CXX_COMPILER variable, the CXX env var is ignored and thus the extra
    # arch specific compiler flags are not picked up anymore, leading to a configuration failure.
    #
    # To avoid this issue, disable automatic embedding of the compilers into the bobui toolchain when
    # cross compiling. This is merely a heuristic, becacuse we don't have enough data to decide
    # when to do it or not.
    # For example on Linux one might want to allow mixing of clang and gcc (maybe).
    #
    # To allow such use cases when the default is wrong, one can provide a flag to explicitly opt-in
    # or opt-out of the compiler embedding into the BobUI toolchain.
    #
    # Passing -DBOBUI_EMBED_TOOLCHAIN_COMPILER=ON  will force embedding of the compilers.
    # Passing -DBOBUI_EMBED_TOOLCHAIN_COMPILER=OFF will disable embedding of the compilers.
    set(__bobui_embed_toolchain_compilers TRUE)
    if(CMAKE_CROSSCOMPILING)
        set(__bobui_embed_toolchain_compilers FALSE)
    endif()
    if(DEFINED BOBUI_EMBED_TOOLCHAIN_COMPILER)
        if(BOBUI_EMBED_TOOLCHAIN_COMPILER)
            set(__bobui_embed_toolchain_compilers TRUE)
        else()
            set(__bobui_embed_toolchain_compilers FALSE)
        endif()
    endif()
    if(__bobui_embed_toolchain_compilers)
        list(APPEND init_platform "
set(__bobui_initial_c_compiler \"${CMAKE_C_COMPILER}\")
set(__bobui_initial_cxx_compiler \"${CMAKE_CXX_COMPILER}\")
if(BOBUI_USE_ORIGINAL_COMPILER AND NOT DEFINED CMAKE_C_COMPILER
        AND EXISTS \"\${__bobui_initial_c_compiler}\")
    set(CMAKE_C_COMPILER \"\${__bobui_initial_c_compiler}\" CACHE STRING \"\")
endif()
if(BOBUI_USE_ORIGINAL_COMPILER AND NOT DEFINED CMAKE_CXX_COMPILER
        AND EXISTS \"\${__bobui_initial_cxx_compiler}\")
    set(CMAKE_CXX_COMPILER \"\${__bobui_initial_cxx_compiler}\" CACHE STRING \"\")
endif()")
    endif()

    unset(init_additional_used_variables)
    if(APPLE)

        # For an iOS simulator_and_device build, we should not explicitly set the sysroot,
        # but let CMake do it's universal build magic to use one sysroot / sdk per-arch.
        # For a single arch / sysroot build, try to use the initially configured sysroot
        # by name.
        #
        # Also allow to opt out just in case.
        #
        # TODO: Figure out if the same should apply to universal macOS builds.

        # We want to preserve the sysroot as an SDK name, instead of the path
        # that CMake transforms it into in Darwin-initialize.cmake, so we pick
        # it out from the cache, where it hasn't been touched by CMake.
        set(cmake_sysroot_name "$CACHE{CMAKE_OSX_SYSROOT}")

        list(LENGTH CMAKE_OSX_ARCHITECTURES _bobui_osx_architectures_count)
        if(cmake_sysroot_name AND (MACOS OR (UIKIT AND NOT _bobui_osx_architectures_count GREATER 1)))
            list(APPEND init_platform "
set(__bobui_initial_apple_sdk \"${BOBUI_APPLE_SDK}\")
if(NOT DEFINED CMAKE_OSX_SYSROOT)
    set(CMAKE_OSX_SYSROOT \"${cmake_sysroot_name}\" CACHE STRING \"\")
endif()")
        endif()

        if(CMAKE_OSX_DEPLOYMENT_TARGET)
            list(APPEND init_platform
                "set(CMAKE_OSX_DEPLOYMENT_TARGET \"${CMAKE_OSX_DEPLOYMENT_TARGET}\" CACHE STRING \"\")")
        endif()

        # Save list of initial architectures BobUI was configured with.
        set(_bobui_osx_architectures_escaped "${CMAKE_OSX_ARCHITECTURES}")
        string(REPLACE ";" "LITERAL_SEMICOLON"
            _bobui_osx_architectures_escaped "${_bobui_osx_architectures_escaped}")
        set(docstring "List of architectures BobUI was built with")
        list(APPEND init_platform
            "set(BOBUI_OSX_ARCHITECTURES \"${_bobui_osx_architectures_escaped}\" CACHE STRING \"${docstring}\")")
        list(APPEND init_platform "")

        # When building another bobui repo, ensure the same list of architectures is used by default.
        # Detection of a bobui repo is done by checking for BOBUI_REPO_MODULE_VERSION which is set in
        # the repo's .cmake.conf file.
        # Most standalone tests will also be built with multiple architectures.
        # Certain tests will be built with a single arch only (like tests/auto/cmake) to avoid
        # issues in the CI when trying to build them on VMs that do not have a universal macOS
        # SDK.
        list(APPEND init_platform
            "# Only build multiple architectures when building BobUI itself. Can be explicitly enabled or disabled.")
        list(APPEND init_platform "if((BOBUI_REPO_MODULE_VERSION AND NOT BOBUI_FORCE_SINGLE_BOBUI_OSX_ARCHITECTURE) OR BOBUI_FORCE_ALL_BOBUI_OSX_ARCHITECTURES)")
        list(APPEND init_platform "    set(__bobui_toolchain_building_bobui_repo TRUE)")
        list(APPEND init_platform "    set(CMAKE_OSX_ARCHITECTURES \"\${BOBUI_OSX_ARCHITECTURES}\" CACHE STRING \"\")")
        list(APPEND init_platform "endif()")
        list(APPEND init_platform "")

        # For macOS user projects, the common case is to default to not specifying any architecture.
        # This means CMake will not pass an -arch flag to the compiler and the compiler will
        # choose the default architecture to build for.
        # On Apple Silicon, CMake will introspect whether it's running under Rosetta and will
        # pass the detected architecture (x86_64 under Rosetta or arm64 natively) to the compiler.
        # This is line with default CMake behavior for user projects.
        # If BobUI was cross-compiled to arm64 from an x86_64 host machine, or vice versa, we need
        # to continue using the same architecture for the user project, otherwise the CMake default
        # might be incorrect depending on which host arch the user project is built on.
        if(NOT UIKIT)
            list(LENGTH CMAKE_OSX_ARCHITECTURES arch_count)
            if(arch_count EQUAL 1 AND CMAKE_CROSSCOMPILING AND CMAKE_SYSTEM_NAME STREQUAL "Darwin")
                list(APPEND init_platform
"if(NOT __bobui_toolchain_building_bobui_repo AND NOT BOBUI_NO_SET_OSX_ARCHITECTURES)"
"    set(CMAKE_OSX_ARCHITECTURES \"${CMAKE_OSX_ARCHITECTURES}\" CACHE STRING \"\")"
"    set(CMAKE_SYSTEM_NAME \"${CMAKE_SYSTEM_NAME}\" CACHE STRING \"\")"
"endif()"
                )
            endif()
        endif()

        # For iOS, we provide a bit more convenience.
        # When the user project is built using the Xcode generator, we only specify the architecture
        # if this is a single architecture BobUI for iOS build. If we wouldn't, invoking just
        # xcodebuild from the command line would try to build with the wrong architecture. Also
        # provide an opt-out option just in case.
        #
        # For a multi-architecture build (so simulator_and_device) we set an explicit
        # architecture for simulator only, via _bobui_internal_set_ios_simulator_arch.
        #
        # When using the Ninja generator, specify the first architecture from BOBUI_OSX_ARCHITECTURES
        # (even with a simulator_and_device BobUI build). This ensures that the default configuration
        # at least tries to build something.
        if(UIKIT)
            bobui_internal_get_first_osx_arch(osx_first_arch)
            list(APPEND init_platform
"if((NOT CMAKE_GENERATOR STREQUAL \"Xcode\" AND NOT __bobui_toolchain_building_bobui_repo)
    OR (CMAKE_GENERATOR STREQUAL \"Xcode\" AND __bobui_initial_apple_sdk
        AND NOT BOBUI_NO_SET_OSX_ARCHITECTURES))")
            list(APPEND init_platform
                "    set(CMAKE_OSX_ARCHITECTURES \"${osx_first_arch}\" CACHE STRING \"\")")
            list(APPEND init_platform "endif()")
            list(APPEND init_platform "")
        endif()

        if(UIKIT)
            list(APPEND init_platform
                "set(CMAKE_SYSTEM_NAME \"${CMAKE_SYSTEM_NAME}\" CACHE STRING \"\")")
        endif()
    elseif(ANDROID)
        list(APPEND init_platform
"# Detect Android SDK/NDK from environment before loading the Android platform toolchain file."
"if(\"$\{ANDROID_SDK_ROOT}\" STREQUAL \"\" AND NOT \"\$ENV{ANDROID_SDK_ROOT}\" STREQUAL \"\")"
"    set(ANDROID_SDK_ROOT \"\$ENV{ANDROID_SDK_ROOT}\" CACHE STRING \"Path to the Android SDK\")"
"endif()"
"if(\"$\{ANDROID_NDK_ROOT}\" STREQUAL \"\" AND NOT \"\$ENV{ANDROID_NDK_ROOT}\" STREQUAL \"\")"
"    set(ANDROID_NDK_ROOT \"\$ENV{ANDROID_NDK_ROOT}\" CACHE STRING \"Path to the Android NDK\")"
"endif()"
        )

        foreach(var ANDROID_PLATFORM ANDROID_NATIVE_API_LEVEL ANDROID_STL
                ANDROID_ABI ANDROID_SDK_ROOT ANDROID_NDK_ROOT)
            list(APPEND init_additional_used_variables
                "list(APPEND __bobui_toolchain_used_variables ${var})")
        endforeach()

        list(APPEND init_platform
            "if(NOT DEFINED ANDROID_PLATFORM AND NOT DEFINED ANDROID_NATIVE_API_LEVEL)")
        list(APPEND init_platform
            "    set(ANDROID_PLATFORM \"${ANDROID_PLATFORM}\" CACHE STRING \"\")")
        list(APPEND init_platform "endif()")
        list(APPEND init_platform "set(ANDROID_STL \"${ANDROID_STL}\" CACHE STRING \"\")")
        list(APPEND init_platform "set(ANDROID_ABI \"${ANDROID_ABI}\" CACHE STRING \"\")")
        list(APPEND init_platform "if (NOT DEFINED ANDROID_SDK_ROOT)")
        file(TO_CMAKE_PATH "${ANDROID_SDK_ROOT}" __bobui_android_sdk_root)
        list(APPEND init_platform
             "    set(ANDROID_SDK_ROOT \"${__bobui_android_sdk_root}\" CACHE STRING \"\")")
        list(APPEND init_platform "endif()")

        list(APPEND init_platform "if(NOT \"$\{ANDROID_NDK_ROOT\}\" STREQUAL \"\")")
        list(APPEND init_platform
            "    set(__bobui_toolchain_file_candidate \"$\{ANDROID_NDK_ROOT\}/build/cmake/android.toolchain.cmake\")")
        list(APPEND init_platform "    if(EXISTS \"$\{__bobui_toolchain_file_candidate\}\")")
        list(APPEND init_platform
            "        message(STATUS \"Android toolchain file within NDK detected: $\{__bobui_toolchain_file_candidate\}\")")
        list(APPEND init_platform "        set(__bobui_chainload_toolchain_file \"$\{__bobui_toolchain_file_candidate\}\")")
        list(APPEND init_platform "    else()")
        list(APPEND init_platform
            "        message(FATAL_ERROR \"Cannot find the toolchain file '$\{__bobui_toolchain_file_candidate\}'. \"")
        list(APPEND init_platform
            "            \"Please specify the toolchain file with -DBOBUI_CHAINLOAD_TOOLCHAIN_FILE=<file>.\")")
        list(APPEND init_platform "    endif()")
        list(APPEND init_platform "endif()")

        bobui_internal_get_android_cmake_policy_version_minimum_assignment(
            android_cmake_policy_version_minimum TYPE TOOLCHAIN_FILE_ASSIGNMENT)
        if(android_cmake_policy_version_minimum)
            list(APPEND init_platform "${android_cmake_policy_version_minimum}")
        endif()

    elseif(EMSCRIPTEN)
        list(APPEND init_platform
"include(\${CMAKE_CURRENT_LIST_DIR}/BobUIPublicWasmToolchainHelpers.cmake)
if(DEFINED ENV{EMSDK} AND NOT \"\$ENV{EMSDK}\" STREQUAL \"\")
    __bobui_internal_get_emroot_path_suffix_from_emsdk_env(__bobui_toolchain_emroot_path)
    __bobui_internal_get_emscripten_cmake_toolchain_file_path_from_emsdk_env(
        \"\${__bobui_toolchain_emroot_path}\" _bobui_candidate_emscripten_toolchain_path)
    set(__bobui_chainload_toolchain_file \"\${_bobui_candidate_emscripten_toolchain_path}\")
endif()
")
        list(APPEND init_post_chainload_toolchain "
if(NOT __bobui_chainload_toolchain_file_included)
    __bobui_internal_show_error_no_emscripten_toolchain_file_found_when_using_bobui()
endif()
")
    endif()

    string(REPLACE ";" "\n" init_additional_used_variables
        "${init_additional_used_variables}")
    string(REPLACE ";" "\n" init_vcpkg "${init_vcpkg}")

    string(REPLACE ";" "\n" init_platform "${init_platform}")
    string(REPLACE "LITERAL_SEMICOLON" ";" init_platform "${init_platform}")

    string(REPLACE ";" "\n" init_post_chainload_toolchain "${init_post_chainload_toolchain}")
    string(REPLACE "LITERAL_SEMICOLON" ";" init_post_chainload_toolchain
           "${init_post_chainload_toolchain}")

    bobui_compute_relative_path_from_cmake_config_dir_to_prefix()
    bobui_re_escape(re_INSTALL_CMAKEDIR "${INSTALL_CMAKEDIR}")
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/bobui.toolchain.cmake.in"
        "${__GlobalConfig_build_dir}/bobui.toolchain.cmake" @ONLY)
    bobui_install(FILES "${__GlobalConfig_build_dir}/bobui.toolchain.cmake"
               DESTINATION "${__GlobalConfig_install_dir}" COMPONENT Devel)
endfunction()
