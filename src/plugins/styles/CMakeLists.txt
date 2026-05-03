# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(BOBUI_FEATURE_style_android)
    add_subdirectory(android)
endif()
if(BOBUI_FEATURE_style_mac)
    add_subdirectory(mac)
endif()
if(BOBUI_FEATURE_style_windowsvista)
    add_subdirectory(modernwindows)
endif()
