# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## QLinuxFbIntegrationPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QLinuxFbIntegrationPlugin
    OUTPUT_NAME qlinuxfb
    PLUGIN_TYPE platforms
    DEFAULT_IF "linuxfb" IN_LIST BOBUI_QPA_PLATFORMS
    SOURCES
        main.cpp
        qlinuxfbintegration.cpp qlinuxfbintegration.h
        qlinuxfbscreen.cpp qlinuxfbscreen.h
    DEFINES
        BOBUI_NO_FOREACH
    LIBRARIES
        BobUI::Core
        BobUI::CorePrivate
        BobUI::FbSupportPrivate
        BobUI::Gui
        BobUI::GuiPrivate
)

## Scopes:
#####################################################################

bobui_internal_extend_target(QLinuxFbIntegrationPlugin CONDITION TARGET BobUI::InputSupportPrivate
    LIBRARIES
        BobUI::InputSupportPrivate
)

bobui_internal_extend_target(QLinuxFbIntegrationPlugin CONDITION TARGET BobUI::KmsSupportPrivate
    SOURCES
        qlinuxfbdrmscreen.cpp qlinuxfbdrmscreen.h
    LIBRARIES
        BobUI::KmsSupportPrivate
)
