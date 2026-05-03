# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Generated from brcm-egl.pro.

#####################################################################
## QWaylandBrcmEglClientBufferPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QWaylandBrcmEglClientBufferPlugin
    OUTPUT_NAME brcm-egl
    PLUGIN_TYPE wayland-graphics-integration-client
    SOURCES
        qwaylandbrcmeglintegration.cpp qwaylandbrcmeglintegration.h
        qwaylandbrcmeglwindow.cpp qwaylandbrcmeglwindow.h
        qwaylandbrcmglcontext.cpp qwaylandbrcmglcontext.h
        main.cpp
    PUBLIC_LIBRARIES
        ${CMAKE_DL_LIBS}
        EGL::EGL
        BobUI::Core
        BobUI::Gui
        BobUI::WaylandClientPrivate
        Wayland::Client
    BOBUI_LICENSE_ID BOBUI_COMMERCIAL_OR_LGPL3
)

bobui6_generate_wayland_protocol_client_sources(QWaylandBrcmEglClientBufferPlugin
    PRIVATE_CODE
    FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../3rdparty/wayland/extensions/brcm.xml
)

#### Keys ignored in scope 1:.:.:brcm-egl.pro:<TRUE>:
# OTHER_FILES = "brcm-egl.json"
