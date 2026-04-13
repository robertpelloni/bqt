# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## QEvdevKeyboardPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QVxKeyboardPlugin
    OUTPUT_NAME qvxkeyboardplugin
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
