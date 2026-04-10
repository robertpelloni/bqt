# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## qscreen Binary:
#####################################################################

bobui_internal_add_manual_test(qscreen
    SOURCES
        main.cpp
        propertyfield.cpp propertyfield.h
        propertywatcher.cpp propertywatcher.h
    NO_PCH_SOURCES
        main.cpp # undef BOBUI_NO_FOREACH
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
)
