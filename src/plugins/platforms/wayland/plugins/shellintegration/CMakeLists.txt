# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Generated from shellintegration.pro.

if(BOBUI_FEATURE_wayland_client_fullscreen_shell_v1)
    add_subdirectory(fullscreen-shell-v1)
endif()
if(BOBUI_FEATURE_wayland_client_wl_shell)
    add_subdirectory(wl-shell)
endif()
if(BOBUI_FEATURE_wayland_client_xdg_shell)
    add_subdirectory(xdg-shell)
endif()
