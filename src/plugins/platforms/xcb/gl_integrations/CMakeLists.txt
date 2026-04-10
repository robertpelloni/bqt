# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(BOBUI_FEATURE_xcb_egl_plugin)
    add_subdirectory(xcb_egl)
endif()
if(BOBUI_FEATURE_xcb_glx_plugin)
    add_subdirectory(xcb_glx)
endif()
