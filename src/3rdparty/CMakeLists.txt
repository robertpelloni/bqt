# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause
if(BOBUI_FEATURE_gui AND BOBUI_FEATURE_png AND NOT BOBUI_FEATURE_system_png)
    add_subdirectory(libpng)
endif()
bobui_install_3rdparty_library_wrap_config_extra_file(BundledLibpng)

if(BOBUI_FEATURE_gui AND BOBUI_FEATURE_jpeg AND NOT BOBUI_FEATURE_system_jpeg)
    add_subdirectory(libjpeg)
endif()
bobui_install_3rdparty_library_wrap_config_extra_file(BundledLibjpeg)

# Needed for freetype, because it links against WrapPNG
bobui_find_package(WrapPNG MODULE PROVIDED_TARGETS WrapPNG::WrapPNG)
if(BOBUI_FEATURE_gui AND BOBUI_FEATURE_freetype AND NOT BOBUI_FEATURE_system_freetype)
    add_subdirectory(freetype)
endif()
bobui_install_3rdparty_library_wrap_config_extra_file(BundledFreetype)

if(BOBUI_FEATURE_gui AND BOBUI_FEATURE_harfbuzz AND NOT BOBUI_FEATURE_system_harfbuzz)
    add_subdirectory(harfbuzz-ng)
endif()
bobui_install_3rdparty_library_wrap_config_extra_file(BundledHarfbuzz)

if (ANDROID)
    add_subdirectory(gradle)
endif()
