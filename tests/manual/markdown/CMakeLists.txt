# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## html2md Binary:
#####################################################################

bobui_internal_add_manual_test(html2md
    GUI
    SOURCES
        html2md.cpp
    INCLUDE_DIRECTORIES
        .
    LIBRARIES
        BobUI::Gui
)
