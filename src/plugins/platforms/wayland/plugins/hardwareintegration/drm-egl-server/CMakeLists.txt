# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Generated from drm-egl-server.pro.

#####################################################################
## DrmEglServerBufferPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(DrmEglServerBufferPlugin
    OUTPUT_NAME drm-egl-server
    PLUGIN_TYPE wayland-graphics-integration-client
    SOURCES
        drmeglserverbufferintegration.cpp drmeglserverbufferintegration.h
        main.cpp
    LIBRARIES
        EGL::EGL
        BobUI::Core
        BobUI::Gui
        BobUI::OpenGL
        BobUI::WaylandClientPrivate
        Wayland::Client
    BOBUI_LICENSE_ID BOBUI_COMMERCIAL_OR_LGPL3
)

bobui6_generate_wayland_protocol_client_sources(DrmEglServerBufferPlugin
    PRIVATE_CODE
    FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../3rdparty/wayland/extensions/drm-egl-server-buffer.xml
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../3rdparty/wayland/extensions/server-buffer-extension.xml
)

#### Keys ignored in scope 1:.:.:drm-egl-server.pro:<TRUE>:
# OTHER_FILES = "drm-egl-server.json"
# QMAKE_CXXFLAGS_WARN_ON = "--Wcast-qual"
