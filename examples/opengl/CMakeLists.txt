# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

bobui_internal_add_example(openglwindow)
bobui_internal_add_example(hellogles3)
if(TARGET BobUI6::Widgets)
    bobui_internal_add_example(2dpainting)
    bobui_internal_add_example(hellogl2)
    bobui_internal_add_example(cube)
    bobui_internal_add_example(textures)
    bobui_internal_add_example(stereoqopenglwidget)
endif()
