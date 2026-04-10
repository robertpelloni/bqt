# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Generated from bradient.pro.

#####################################################################
## QWaylandBradientDecorationPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QWaylandBradientDecorationPlugin
    OUTPUT_NAME bradient
    PLUGIN_TYPE wayland-decoration-client
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::Core
        BobUI::Gui
        BobUI::WaylandClientPrivate
        Wayland::Client
    BOBUI_LICENSE_ID BOBUI_COMMERCIAL_OR_LGPL3
)

#### Keys ignored in scope 1:.:.:bradient.pro:<TRUE>:
# OTHER_FILES = "bradient.json"
