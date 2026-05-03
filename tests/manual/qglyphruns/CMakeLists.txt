# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_manual_test(qglyphrun
    GUI
    SOURCES
        main.cpp
        controller.h controller.cpp controller.ui
        view.h view.cpp
        glyphruninspector.h glyphruninspector.cpp
        singleglyphrun.h singleglyphrun.cpp singleglyphrun.ui
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
    ENABLE_AUTOGEN_TOOLS
        uic
)
