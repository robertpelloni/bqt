# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## QQnxIntegrationPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QQnxIntegrationPlugin
    OUTPUT_NAME qqnx
    PLUGIN_TYPE platforms
    DEFAULT_IF "qnx" IN_LIST BOBUI_QPA_PLATFORMS
    SOURCES
        main.cpp main.h
        qqnxabstractcover.h
        qqnxabstractnavigator.cpp qqnxabstractnavigator.h
        qqnxabstractvirtualkeyboard.cpp qqnxabstractvirtualkeyboard.h
        qqnxbuffer.cpp qqnxbuffer.h
        qqnxcursor.cpp qqnxcursor.h
        qqnxforeignwindow.cpp qqnxforeignwindow.h
        qqnxglobal.cpp qqnxglobal.h
        qqnxintegration.cpp qqnxintegration.h
        qqnxkeytranslator.h
        qqnxlgmon.h
        qqnxnativeinterface.cpp qqnxnativeinterface.h
        qqnxnavigatoreventhandler.cpp qqnxnavigatoreventhandler.h
        qqnxrasterbackingstore.cpp qqnxrasterbackingstore.h
        qqnxrasterwindow.cpp qqnxrasterwindow.h
        qqnxscreen.cpp qqnxscreen.h
        qqnxscreeneventfilter.h
        qqnxscreeneventhandler.cpp qqnxscreeneventhandler.h
        qqnxscreeneventthread.cpp qqnxscreeneventthread.h
        qqnxservices.cpp qqnxservices.h
        qqnxwindow.cpp qqnxwindow.h
    NO_PCH_SOURCES
        qqnxclipboard.cpp # undef BOBUI_NO_FOREACH
        qqnxintegration.cpp # undef BOBUI_NO_FOREACH
        qqnxscreen.cpp # undef BOBUI_NO_FOREACH
        qqnxscreeneventhandler.cpp # undef BOBUI_NO_FOREACH
        qqnxwindow.cpp # undef BOBUI_NO_FOREACH
    LIBRARIES
        BobUI::Core
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
        screen
)

## Scopes:
#####################################################################

bobui_internal_extend_target(QQnxIntegrationPlugin CONDITION BOBUI_FEATURE_egl
    SOURCES
        qqnxeglwindow.cpp qqnxeglwindow.h
        qqnxglcontext.cpp qqnxglcontext.h
    LIBRARIES
        EGL::EGL
)

bobui_internal_extend_target(QQnxIntegrationPlugin CONDITION BOBUI_FEATURE_qqnx_pps
    SOURCES
        qqnxbuttoneventnotifier.cpp qqnxbuttoneventnotifier.h
        qqnxnavigatoreventnotifier.cpp qqnxnavigatoreventnotifier.h
        qqnxnavigatorpps.cpp qqnxnavigatorpps.h
        qqnxvirtualkeyboardpps.cpp qqnxvirtualkeyboardpps.h
    LIBRARIES
        PPS::PPS
)

bobui_internal_extend_target(QQnxIntegrationPlugin CONDITION BOBUI_FEATURE_clipboard AND BOBUI_FEATURE_qqnx_pps
    SOURCES
        qqnxclipboard.cpp qqnxclipboard.h
    LIBRARIES
        clipboard
)

bobui_internal_extend_target(QQnxIntegrationPlugin CONDITION BOBUI_FEATURE_qqnx_imf AND BOBUI_FEATURE_qqnx_pps
    SOURCES
        qqnxinputcontext_imf.cpp qqnxinputcontext_imf.h
)

bobui_internal_extend_target(QQnxIntegrationPlugin CONDITION BOBUI_FEATURE_qqnx_pps AND NOT BOBUI_FEATURE_qqnx_imf
    SOURCES
        qqnxinputcontext_noimf.cpp qqnxinputcontext_noimf.h
)

bobui_internal_extend_target(QQnxIntegrationPlugin CONDITION lgmon
    SOURCES
        qqnxlgmon.cpp
    DEFINES
        QQNX_LGMON
    LIBRARIES
        lgmon
)
