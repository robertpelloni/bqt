# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

bobui_internal_add_example(hellovulkantriangle)
if(TARGET BobUI6::Widgets)
    bobui_internal_add_example(hellovulkanwidget)
endif()
if(TARGET BobUI6::Concurrent AND TARGET BobUI6::Widgets)
    bobui_internal_add_example(hellovulkancubes)
endif()
