# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_find_package(GTK3 MODULE)
bobui_find_package(X11 MODULE)

#####################################################################
## gtk-container Binary:
#####################################################################

bobui_internal_add_manual_test(gtk-container
    GUI
    SOURCES
        gtk-container.cpp
        DEFINES
        GDK_VERSION_MIN_REQUIRED=GDK_VERSION_3_6
    LIBRARIES
        PkgConfig::GTK3
        BobUI::Core
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
        X11::X11
)
