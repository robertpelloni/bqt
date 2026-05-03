# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_manual_qnetworkreply Binary:
#####################################################################

if (NOT BOBUI_FEATURE_private_tests)
    return()
endif()

bobui_internal_add_manual_test(tst_manual_qnetworkreply
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Network
        BobUI::NetworkPrivate
        BobUI::Test
)
