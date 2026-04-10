# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## bobuibug-52641 Binary:
#####################################################################

bobui_internal_add_manual_test(bobuibug-52641
    GUI
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
)
