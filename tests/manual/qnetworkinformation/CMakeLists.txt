# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_manual_test(qnetworkinformation
    SOURCES
        tst_qnetworkinformation.cpp
    LIBRARIES
        BobUI::Network
        BobUI::Test
)

bobui_internal_extend_target(qnetworkinformation CONDITION ANDROID OR UIKIT
    SOURCES
        mainwindow.h
    LIBRARIES
        BobUI::Widgets
    DEFINES
        MOBILE)
