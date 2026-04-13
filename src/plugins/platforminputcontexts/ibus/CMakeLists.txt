# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## QIbusPlatformInputContextPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QIbusPlatformInputContextPlugin
    OUTPUT_NAME ibusplatforminputcontextplugin
    PLUGIN_TYPE platforminputcontexts
    DEFAULT_IF FALSE
    SOURCES
        main.cpp
        qibusinputcontextproxy.cpp qibusinputcontextproxy.h
        qibusplatforminputcontext.cpp qibusplatforminputcontext.h
        qibusproxy.cpp qibusproxy.h
        qibusproxyportal.cpp qibusproxyportal.h
        qibustypes.cpp qibustypes.h
    LIBRARIES
        BobUI::Core
        BobUI::DBus
        BobUI::Gui
        BobUI::GuiPrivate
        XKB::XKB
)
