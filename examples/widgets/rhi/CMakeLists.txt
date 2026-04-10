# Copyright (C) 2023 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

if(NOT TARGET BobUI6::Widgets)
    return()
endif()
bobui_internal_add_example(simplerhiwidget)
bobui_internal_add_example(cuberhiwidget)
