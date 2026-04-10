# Copyright (C) 2025 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

macro(bobui_internal_setup_cmake_and_export_namespace)
    # The variables might have already been set in BobUIBuildInternalsExtra.cmake if the file is
    # included while building a new module and not BobUIBase. In that case, stop overriding the value.
    if(NOT INSTALL_CMAKE_NAMESPACE)
        set(INSTALL_CMAKE_NAMESPACE "BobUI${PROJECT_VERSION_MAJOR}"
            CACHE STRING "CMake namespace [BobUI${PROJECT_VERSION_MAJOR}]")
    endif()
    if(NOT BOBUI_CMAKE_EXPORT_NAMESPACE)
        set(BOBUI_CMAKE_EXPORT_NAMESPACE "BobUI${PROJECT_VERSION_MAJOR}"
            CACHE STRING "CMake namespace used when exporting targets [BobUI${PROJECT_VERSION_MAJOR}]")
    endif()
endmacro()
