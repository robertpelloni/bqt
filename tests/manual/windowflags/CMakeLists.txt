# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_manual_windowflags Binary:
#####################################################################

bobui_internal_add_manual_test(tst_manual_windowflags
    GUI
    SOURCES
        controllerwindow.cpp controllerwindow.h
        controls.cpp controls.h
        main.cpp
        previewwindow.cpp previewwindow.h
    NO_PCH_SOURCES
        controls.cpp # undef BOBUI_NO_FOREACH
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::Widgets
)
