# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Generated from fullscreen-shell-v1.pro.

#####################################################################
## QWaylandFullScreenShellV1IntegrationPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QWaylandFullScreenShellV1IntegrationPlugin
    OUTPUT_NAME fullscreen-shell-v1
    PLUGIN_TYPE wayland-shell-integration
    SOURCES
        main.cpp
        qwaylandfullscreenshellv1integration.cpp qwaylandfullscreenshellv1integration.h
        qwaylandfullscreenshellv1surface.cpp qwaylandfullscreenshellv1surface.h
    LIBRARIES
        BobUI::Core
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::WaylandClientPrivate
        Wayland::Client
    BOBUI_LICENSE_ID BOBUI_COMMERCIAL_OR_LGPL3
    ATTRIBUTION_FILE_DIR_PATHS
        ../../../../../../3rdparty/wayland/protocols/fullscreen-shell
)

bobui6_generate_wayland_protocol_client_sources(QWaylandFullScreenShellV1IntegrationPlugin
    PRIVATE_CODE
    FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../3rdparty/wayland/protocols/fullscreen-shell/fullscreen-shell-unstable-v1.xml
)

#### Keys ignored in scope 1:.:.:fullscreen-shell-v1.pro:<TRUE>:
# OTHER_FILES = "fullscreen-shell-v1.json"
