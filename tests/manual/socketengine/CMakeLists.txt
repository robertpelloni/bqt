# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_socketengine Binary:
#####################################################################

if (NOT BOBUI_FEATURE_private_tests)
    return()
endif()

bobui_internal_add_manual_test(tst_socketengine
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::NetworkPrivate
        BobUI::Test
)
