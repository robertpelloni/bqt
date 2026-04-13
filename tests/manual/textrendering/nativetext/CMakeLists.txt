# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## nativetext Binary:
#####################################################################

bobui_internal_add_manual_test(nativetext
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::Widgets
)

## Scopes:
#####################################################################

bobui_internal_extend_target(nativetext CONDITION APPLE
    LIBRARIES
        ${FWAppKit}
        ${FWCoreGraphics}
        ${FWFoundation}
    COMPILE_OPTIONS
        -x
        objective-c++
)
