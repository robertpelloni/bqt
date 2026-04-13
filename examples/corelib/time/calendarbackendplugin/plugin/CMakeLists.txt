# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

bobui_add_library(calendarPlugin SHARED
    ../common/calendarBackendInterface.h
    calendarplugin.h
    calendarplugin.cpp
    calendarbackend.cpp
    calendarbackend.h
)

target_include_directories(calendarPlugin PRIVATE ../common)

target_link_libraries(calendarPlugin
    PRIVATE
        BobUI::Widgets
        BobUI::Core
        BobUI::CorePrivate
)
