# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## event_compression Binary:
#####################################################################

bobui_internal_add_manual_test(event_compression
    SOURCES
        main.cpp
        mousestatwidget.cpp mousestatwidget.h
    LIBRARIES
        BobUI::Gui
        BobUI::Test
        BobUI::Widgets
)
