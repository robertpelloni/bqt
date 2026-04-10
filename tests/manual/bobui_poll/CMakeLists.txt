# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bobui_poll Test:
#####################################################################

bobui_internal_add_test(tst_bobui_poll
    SOURCES
        ../../../src/corelib/kernel/qpoll.cpp
        tst_bobui_poll.cpp
    INCLUDE_DIRECTORIES
        ../../../src/corelib/kernel
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Network
)
