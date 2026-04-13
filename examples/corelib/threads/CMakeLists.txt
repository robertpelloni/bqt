# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

if(NOT ANDROID)
    bobui_internal_add_example(semaphores)
    bobui_internal_add_example(waitconditions)
endif()
if(TARGET BobUI6::Widgets)
    bobui_internal_add_example(mandelbrot)
    bobui_internal_add_example(queuedcustomtype)
endif()
