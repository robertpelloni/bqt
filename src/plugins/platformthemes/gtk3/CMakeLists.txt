# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_find_package(GTK3 MODULE)

if(BOBUI_FEATURE_xlib)
    bobui_find_package(X11 MODULE)
endif()

#####################################################################
## QGtk3ThemePlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QGtk3ThemePlugin
    OUTPUT_NAME qgtk3
    PLUGIN_TYPE platformthemes
    DEFAULT_IF FALSE
    NO_UNITY_BUILD
    SOURCES
        main.cpp
        qgtk3dialoghelpers.cpp qgtk3dialoghelpers.h

        qgtk3theme.cpp qgtk3theme.h
        qgtk3interface.cpp qgtk3interface_p.h
        qgtk3storage.cpp qgtk3storage_p.h
        qgtk3json.cpp qgtk3json_p.h
    NO_PCH_SOURCES
        qgtk3dialoghelpers.cpp # undef BOBUI_NO_FOREACH
    DEFINES
        GDK_VERSION_MIN_REQUIRED=GDK_VERSION_3_6
    LIBRARIES
        PkgConfig::GTK3
        BobUI::Core
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
)

bobui_internal_extend_target(QGtk3ThemePlugin CONDITION BOBUI_FEATURE_dbus
    LIBRARIES
        BobUI::DBus
)

bobui_internal_extend_target(QGtk3ThemePlugin CONDITION BOBUI_FEATURE_xlib
    LIBRARIES
        X11::X11
)
