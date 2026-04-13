# Copyright (C) 2023 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

add_library(bindableproperties_shared STATIC
    subscriptionwindow.cpp
    subscriptionwindow.h
    subscriptionwindow.ui
)

target_link_libraries(bindableproperties_shared PUBLIC
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Widgets
)

bobui_add_resources(bindableproperties_shared "countries"
    PREFIX
        "/"
    FILES
        "finland.png"
        "germany.png"
        "norway.png"
)
