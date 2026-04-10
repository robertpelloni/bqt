# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

bobui_add_executable(JulianGregorianCalendar
    ../common/calendarBackendInterface.h
    main.cpp
)

target_include_directories(JulianGregorianCalendar PRIVATE ../common)

target_link_libraries(JulianGregorianCalendar
    PRIVATE
        BobUI::Widgets
        BobUI::Core
)

