# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Generated from dmabuf-server.pro.

#####################################################################
## DmaBufServerBufferPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(DmaBufServerBufferPlugin
    OUTPUT_NAME dmabuf-server
    PLUGIN_TYPE wayland-graphics-integration-client
    SOURCES
        dmabufserverbufferintegration.cpp dmabufserverbufferintegration.h
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

bobui6_generate_wayland_protocol_client_sources(DmaBufServerBufferPlugin
    PRIVATE_CODE
    FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../3rdparty/wayland/extensions/bobui-dmabuf-server-buffer.xml
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../3rdparty/wayland/extensions/server-buffer-extension.xml
)

#### Keys ignored in scope 1:.:.:dmabuf-server.pro:<TRUE>:
# OTHER_FILES = "dmabuf-server.json"
# QMAKE_CXXFLAGS_WARN_ON = "--Wcast-qual"
