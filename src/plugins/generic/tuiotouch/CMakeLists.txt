# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## BOBUIuioTouchPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(BOBUIuioTouchPlugin
    OUTPUT_NAME bobuiuiotouchplugin
    PLUGIN_TYPE generic
    DEFAULT_IF FALSE
    SOURCES
        main.cpp
        qoscbundle.cpp qoscbundle_p.h
        qoscmessage.cpp qoscmessage_p.h
        bobuiuiocursor_p.h
        bobuiuiohandler.cpp bobuiuiohandler_p.h
        bobuiuiotoken_p.h
    DEFINES
        BOBUI_NO_FOREACH
    LIBRARIES
        BobUI::Core
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::Network
)
