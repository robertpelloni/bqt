# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## QWindowsVistaStylePlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QModernWindowsStylePlugin
    OUTPUT_NAME qmodernwindowsstyle
    PLUGIN_TYPE styles
    SOURCES
        main.cpp
        qwindowsvistastyle.cpp qwindowsvistastyle_p.h
        qwindows11style.cpp qwindows11style_p.h
        qwindowsvistastyle_p_p.h
        qwindowsvistaanimation.cpp qwindowsvistaanimation_p.h
        qwindowsthemedata.cpp qwindowsthemedata_p.h

    LIBRARIES
        dwmapi
        gdi32
        user32
        uxtheme
        BobUI::Core
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::WidgetsPrivate
)
