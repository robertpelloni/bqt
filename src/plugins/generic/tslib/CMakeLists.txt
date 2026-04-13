# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_find_package(Tslib MODULE)

#####################################################################
## BOBUIsLibPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(BOBUIsLibPlugin
    OUTPUT_NAME bobuislibplugin
    PLUGIN_TYPE generic
    DEFAULT_IF FALSE
    SOURCES
        main.cpp
    LIBRARIES
        PkgConfig::Tslib
        BobUI::Core
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::InputSupportPrivate
)
