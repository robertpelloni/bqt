# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## QVncIntegrationPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QVncIntegrationPlugin
    OUTPUT_NAME qvnc
    PLUGIN_TYPE platforms
    DEFAULT_IF "vnc" IN_LIST BOBUI_QPA_PLATFORMS
    SOURCES
        main.cpp
        qvnc.cpp qvnc_p.h
        qvncclient.cpp qvncclient.h
        qvncintegration.cpp qvncintegration.h
        qvncscreen.cpp qvncscreen.h
    DEFINES
        BOBUI_NO_FOREACH
    LIBRARIES
        BobUI::Core
        BobUI::CorePrivate
        BobUI::FbSupportPrivate
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::Network
)

## Scopes:
#####################################################################

bobui_internal_extend_target(QVncIntegrationPlugin CONDITION TARGET BobUI::InputSupportPrivate
    LIBRARIES
        BobUI::InputSupportPrivate
)
