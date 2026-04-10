# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## bobuiimer_vs_qmetaobject Binary:
#####################################################################

bobui_internal_add_benchmark(bobuiimer_vs_qmetaobject
    SOURCES
        tst_bobuiimer_vs_qmetaobject.cpp
    INCLUDE_DIRECTORIES
        .
    LIBRARIES
        BobUI::Test
)
