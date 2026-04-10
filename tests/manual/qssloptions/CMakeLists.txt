# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qssloptions Binary:
#####################################################################

bobui_internal_add_manual_test(tst_qssloptions
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::Network
        BobUI::Test
)
