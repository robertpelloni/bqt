# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## qcursorhighdpi Binary:
#####################################################################

bobui_internal_add_manual_test(qcursorhighdpi
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

bobui_internal_extend_target(qcursorhighdpi CONDITION WIN32
    LIBRARIES
        user32
)
