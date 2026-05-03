# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Macro that checks for a single repo target set build, and returns from the current file, directory
# or function. Use this at the top of project files to whitelist the file for the given package.
macro(bobui_internal_include_in_repo_target_set _repo_target_set_name)
    if(DEFINED BOBUI_BUILD_SINGLE_REPO_TARGET_SET)
        if(NOT "${_repo_target_set_name}" STREQUAL BOBUI_BUILD_SINGLE_REPO_TARGET_SET)
            message(STATUS "Not part of repo target set ${BOBUI_BUILD_SINGLE_REPO_TARGET_SET}: "
                "${CMAKE_CURRENT_LIST_DIR}")
            return()
        endif()
    endif()
endmacro()
