# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause
bobui_internal_add_manual_test(echo_client_mainthread
    GUI
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::Core
        BobUI::Network
)
