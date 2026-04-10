# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_create_depends_files()
bobui_generate_build_internals_extra_cmake_code()
bobui_internal_create_plugins_auto_inclusion_files()
bobui_internal_create_config_file_for_standalone_tests()

# Needs to run after bobui_internal_create_depends_files.
bobui_create_tools_config_files()

if (ANDROID)
    bobui_modules_process_android_dependencies()
endif()

bobui_internal_generate_user_facing_tools_info()
