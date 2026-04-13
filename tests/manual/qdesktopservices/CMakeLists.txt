# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_manual_qdesktopservices Binary:
#####################################################################

bobui_internal_add_manual_test(tst_manual_qdesktopservices
    SOURCES
        tst_qdesktopservices.cpp
    DEFINES
        SRCDIR="${CMAKE_CURRENT_SOURCE_DIR}/"
    LIBRARIES
        BobUI::Gui
        BobUI::Test
)
