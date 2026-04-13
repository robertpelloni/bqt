# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## t_cursors Binary:
#####################################################################

bobui_internal_add_manual_test(t_cursors
    GUI
    SOURCES
        main.cpp
        mainwindow.cpp mainwindow.h mainwindow.ui
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
    ENABLE_AUTOGEN_TOOLS
        uic
)

# Resources:
set(images_resource_files
    "data/monkey_on_64x64.png"
)

bobui_internal_add_resource(t_cursors "images"
    PREFIX
        "/"
    FILES
        ${images_resource_files}
)
