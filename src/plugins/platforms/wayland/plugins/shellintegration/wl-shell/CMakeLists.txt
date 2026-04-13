# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Generated from wl-shell.pro.

#####################################################################
## WlShellIntegrationPrivate Module:
#####################################################################

bobui_internal_add_module(WlShellIntegrationPrivate
    CONFIG_MODULE_NAME wl_shell_integration
    INTERNAL_MODULE
    SOURCES
        qwaylandwlshellintegration.cpp qwaylandwlshellintegration_p.h
        qwaylandwlshellsurface.cpp qwaylandwlshellsurface_p.h
    LIBRARIES
        BobUI::GuiPrivate
        BobUI::WaylandClientPrivate
        Wayland::Client
    PRIVATE_HEADER_FILTERS
        "^qwayland-.*\.h|^wayland-.*-protocol\.h"
    NO_GENERATE_CPP_EXPORTS
    BOBUI_LICENSE_ID BOBUI_COMMERCIAL_OR_LGPL3
    ATTRIBUTION_FILE_DIR_PATHS
        ../../../../../../3rdparty/wayland/protocols/wayland
)

bobui6_generate_wayland_protocol_client_sources(WlShellIntegrationPrivate
    # this is just a helper module and still part of the BobUIWaylandClient build
    __BOBUI_INTERNAL_WAYLAND_INCLUDE_DIR BobUIWaylandClient/private
    PRIVATE_CODE
    FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../3rdparty/wayland/protocols/wayland/wayland.xml
)

#### Keys ignored in scope 2:.:.:wl-shell-integration.pro:<TRUE>:
# MODULE = "wl_shell_integration"

## Scopes:
#####################################################################

bobui_internal_extend_target(WlShellIntegrationPrivate CONDITION BOBUI_FEATURE_xkbcommon
    LIBRARIES
        XKB::XKB
)
#####################################################################
## QWaylandWlShellIntegrationPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QWaylandWlShellIntegrationPlugin
    OUTPUT_NAME wl-shell-plugin
    PLUGIN_TYPE wayland-shell-integration
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::GuiPrivate
        BobUI::WaylandClientPrivate
        BobUI::WlShellIntegrationPrivate
)

#### Keys ignored in scope 4:.:.:wl-shell-plugin.pro:<TRUE>:
# OTHER_FILES = "wl-shell.json"
