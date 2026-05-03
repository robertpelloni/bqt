# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_manual_test(main_noexec
    GUI
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::Core
        BobUI::Gui
)
