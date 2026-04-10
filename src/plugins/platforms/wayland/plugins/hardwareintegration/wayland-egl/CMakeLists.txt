# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Generated from wayland-egl.pro.

#####################################################################
## QWaylandEglClientBufferPlugin Plugin:
#####################################################################
bobui_find_package(EGL MODULE)

bobui_internal_add_plugin(QWaylandEglClientBufferPlugin
    OUTPUT_NAME bobui-plugin-wayland-egl
    PLUGIN_TYPE wayland-graphics-integration-client
    SOURCES
        main.cpp
        qwaylandeglclientbufferintegration.cpp qwaylandeglclientbufferintegration_p.h
        qwaylandeglinclude_p.h
        qwaylandeglwindow.cpp qwaylandeglwindow_p.h
        qwaylandglcontext.cpp qwaylandglcontext_p.h
    LIBRARIES
        ${CMAKE_DL_LIBS}
        EGL::EGL
        BobUI::Core
        BobUI::Gui
        BobUI::OpenGLPrivate
        BobUI::WaylandClientPrivate
        Wayland::Client
        Wayland::Egl
    BOBUI_LICENSE_ID BOBUI_COMMERCIAL_OR_LGPL3
)

bobui_internal_extend_target(QWaylandEglClientBufferPlugin CONDITION BOBUI_FEATURE_egl AND NOT BOBUI_FEATURE_egl_x11
    DEFINES
        BOBUI_EGL_NO_X11
)


#### Keys ignored in scope 1:.:.:wayland-egl.pro:<TRUE>:
# OTHER_FILES = "wayland-egl.json"
