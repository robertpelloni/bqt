# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

bobui_internal_add_example(chip)
bobui_internal_add_example(elasticnodes)
bobui_internal_add_example(collidingmice)
bobui_internal_add_example(basicgraphicslayouts)
bobui_internal_add_example(diagramscene)
bobui_internal_add_example(simpleanchorlayout)
if(BOBUI_FEATURE_cursor AND BOBUI_FEATURE_draganddrop)
    bobui_internal_add_example(dragdroprobot)
endif()
