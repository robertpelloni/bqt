# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

function(bobui_internal_check_msvc_versions)
    if(NOT MSVC OR BOBUI_NO_MSVC_MIN_VERSION_CHECK)
        return()
    endif()
    set(min_msvc_version "1930")
    if(MSVC_VERSION VERSION_LESS min_msvc_version)
        message(FATAL_ERROR
            "BobUI requires at least Visual Studio 2022 (MSVC ${min_msvc_version} or newer), "
            "you're building against version ${MSVC_VERSION}. "
            "You can turn off this version check by setting BOBUI_NO_MSVC_MIN_VERSION_CHECK to ON."
        )
    endif()
endfunction()
