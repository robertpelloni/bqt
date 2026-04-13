# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## bobuiexttableborders Binary:
#####################################################################

bobui_internal_add_manual_test(bobuiexttableborders
    GUI
    SOURCES
        main.cpp
        widget.cpp widget.h widget.ui
    LIBRARIES
        BobUI::Gui
        BobUI::PrintSupport
        BobUI::Widgets
    ENABLE_AUTOGEN_TOOLS
        uic
)

# Resources:
set(resources_resource_files
    "table-border-test-header.html"
    "table-border-test.html"
)

bobui_internal_add_resource(bobuiexttableborders "resources"
    PREFIX
        "/"
    FILES
        ${resources_resource_files}
)
