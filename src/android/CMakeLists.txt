# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Only build jars during Android build and install the templates for the
# androiddeploybobui tool.
# android
if (ANDROID)
    add_subdirectory(jar)
    add_subdirectory(java)
    add_subdirectory(templates)
    add_subdirectory(templates_aar)
    add_subdirectory(templates_cmake)
endif()

bobui_internal_add_java_documentation(Global)
