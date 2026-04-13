# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## weatheranchorlayout Binary:
#####################################################################
cmake_minimum_required(VERSION 3.16)

bobui_internal_add_manual_test(weatheranchorlayout
    GUI
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
)

# Resources:
set(weatheranchorlayout_resource_files
    "images/5days.jpg"
    "images/details.jpg"
    "images/place.jpg"
    "images/tabbar.jpg"
    "images/title.jpg"
    "images/weather-few-clouds.png"
)

bobui_add_resources(weatheranchorlayout "weatheranchorlayout"
    PREFIX
        "/"
    FILES
        ${weatheranchorlayout_resource_files}
)

