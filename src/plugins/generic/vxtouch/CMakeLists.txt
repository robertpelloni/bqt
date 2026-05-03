# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## QVxTouchScreenPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QVxTouchScreenPlugin
    OUTPUT_NAME qvxtouchplugin
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
