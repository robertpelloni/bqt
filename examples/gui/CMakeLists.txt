# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

if(NOT TARGET BobUI6::Gui)
    return()
endif()
bobui_internal_add_example(rasterwindow)
bobui_internal_add_example(rhiwindow)
