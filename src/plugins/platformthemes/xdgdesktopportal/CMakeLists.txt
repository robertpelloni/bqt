# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## QXdgDesktopPortalThemePlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QXdgDesktopPortalThemePlugin
    OUTPUT_NAME qxdgdesktopportal
    PLUGIN_TYPE platformthemes
    DEFAULT_IF FALSE
    SOURCES
        main.cpp
        qxdgdesktopportalfiledialog.cpp qxdgdesktopportalfiledialog_p.h
        qxdgdesktopportaltheme.cpp qxdgdesktopportaltheme.h
    LIBRARIES
        BobUI::Core
        BobUI::CorePrivate
        BobUI::DBus
        BobUI::Gui
        BobUI::GuiPrivate
)
