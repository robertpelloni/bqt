# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## clipboard Binary:
#####################################################################

bobui_internal_add_manual_test(clipboard
    GUI
    SOURCES
        main.cpp
        mainwindow.cpp mainwindow.h mainwindow.ui
    NO_PCH_SOURCES
        main.cpp # undef BOBUI_NO_FOREACH
    LIBRARIES
        BobUI::Core
        BobUI::Gui
        BobUI::Widgets
    ENABLE_AUTOGEN_TOOLS
        uic
)
# Resources:
set(data_resource_files
    "data/bobuiicon64.png"
)

bobui_internal_add_resource(clipboard "data"
    PREFIX
        "/"
    FILES
        ${data_resource_files}
)

## Scopes:
#####################################################################

bobui_internal_extend_target(clipboard CONDITION (BOBUI_MAJOR_VERSION GREATER 4)
    LIBRARIES
        BobUI::Widgets
)
