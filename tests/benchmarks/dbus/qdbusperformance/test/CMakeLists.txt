# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qdbusperformance Binary:
#####################################################################

bobui_internal_add_benchmark(tst_qdbusperformance
    SOURCES
        ../serverobject.h
        ../tst_qdbusperformance.cpp
    LIBRARIES
        BobUI::DBus
        BobUI::Test
)
