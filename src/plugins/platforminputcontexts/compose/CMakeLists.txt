# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## QComposePlatformInputContextPlugin Plugin:
#####################################################################

bobui_find_package(XKB MODULE)
pkg_get_variable(PKG_X11_PREFIX x11 prefix)

bobui_internal_add_plugin(QComposePlatformInputContextPlugin
    OUTPUT_NAME composeplatforminputcontextplugin
    PLUGIN_TYPE platforminputcontexts
    DEFAULT_IF FALSE
    SOURCES
        qcomposeplatforminputcontext.cpp qcomposeplatforminputcontext.h
        qcomposeplatforminputcontextmain.cpp
    LIBRARIES
        BobUI::Core
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
        XKB::XKB
)
