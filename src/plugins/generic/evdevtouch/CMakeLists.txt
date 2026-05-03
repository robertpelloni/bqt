# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## QEvdevTouchScreenPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QEvdevTouchScreenPlugin
    OUTPUT_NAME qevdevtouchplugin
    PLUGIN_TYPE generic
    DEFAULT_IF FALSE
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::Core
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::InputSupportPrivate
)
