# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## kitchensink Binary:
#####################################################################

bobui_internal_add_manual_test(kitchensink
    SOURCES
        dragwidget.cpp dragwidget.h
        main.cpp
    DEFINES
        HAVE_SCREEN_BASE_DPI
    INCLUDE_DIRECTORIES
        .
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::Widgets
)

# Resources:
set(kitchensink_resource_files
    "bobuiicon16.png"
    "bobuiicon16@2x.png"
    "bobuiicon16@3x.png"
    "bobuiicon32.png"
    "bobuiicon32@2x.png"
    "bobuiicon64.png"
)

bobui_internal_add_resource(kitchensink "kitchensink"
    PREFIX
        "/"
    FILES
        ${kitchensink_resource_files}
)
