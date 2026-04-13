# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## QICOPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QICOPlugin
    OUTPUT_NAME qico
    PLUGIN_TYPE imageformats
    SOURCES
        main.cpp
        qicohandler.cpp qicohandler.h
    LIBRARIES
        BobUI::Core
        BobUI::CorePrivate
        BobUI::Gui
    NO_UNITY_BUILD
)
