# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## QAndroidStylePlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QAndroidStylePlugin
    OUTPUT_NAME qandroidstyle
    PLUGIN_TYPE styles
    SOURCES
        main.cpp
        qandroidstyle.cpp qandroidstyle_p.h
    LIBRARIES
        BobUI::Core
        BobUI::Gui
        BobUI::WidgetsPrivate
)
