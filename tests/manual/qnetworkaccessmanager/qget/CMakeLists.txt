# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## qget Binary:
#####################################################################

bobui_internal_add_manual_test(qget
    SOURCES
        downloadmanager.cpp
        qget.cpp qget.h
        transferitem.cpp
    NO_PCH_SOURCES
        downloadmanager.cpp # undef BOBUI_NO_FOREACH
        qget.cpp # undef BOBUI_NO_FOREACH
    LIBRARIES
        BobUI::Network
)
