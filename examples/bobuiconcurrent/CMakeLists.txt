# Copyright (C) 2023 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

if(NOT TARGET BobUI6::Concurrent)
    return()
endif()
if(TARGET BobUI6::Widgets)
    bobui_internal_add_example(imagescaling)
    bobui_internal_add_example(primecounter)
    bobui_internal_add_example(wordcount)
endif()
