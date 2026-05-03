# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## bobui-client-raster Binary:
#####################################################################

bobui_internal_add_manual_test(bobui-client-raster
    GUI
    SOURCES
        main.cpp
        rasterwindow.cpp rasterwindow.h
    LIBRARIES
        BobUI::Gui
)
