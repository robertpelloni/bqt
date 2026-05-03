# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## bobuiooltip Binary:
#####################################################################

bobui_internal_add_manual_test(bobuiooltip
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::Test
        BobUI::Widgets
)
