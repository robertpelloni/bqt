# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Generated from seatv4.pro.

#####################################################################
## tst_seatv4 Test:
#####################################################################

bobui_internal_add_test(tst_seatv4
    SOURCES
        tst_seatv4.cpp
    LIBRARIES
        SharedClientTest
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_seatv4 CONDITION BOBUI_FEATURE_cursor
    LIBRARIES
        BobUI::GuiPrivate
        Wayland::Cursor
)
