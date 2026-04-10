# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## jpeg Binary:
#####################################################################

bobui_internal_add_benchmark(jpeg
    SOURCES
        jpeg.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::Test
)
