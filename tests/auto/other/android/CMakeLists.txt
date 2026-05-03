# Copyright (C) 2025 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

add_subdirectory(deployment_settings)
add_subdirectory(permissions)

if(BOBUI_USE_TARGET_ANDROID_BUILD_DIR)
    add_subdirectory(package_source_dir)
    if(BOBUI_USE_ANDROID_MODERN_BUNDLE)
        add_subdirectory(dynamic_feature)
    endif()
endif()
