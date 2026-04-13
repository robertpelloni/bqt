# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(WIN32 AND BOBUI_FEATURE_networklistmanager)
    add_subdirectory(networklistmanager)
endif()

if(LINUX AND TARGET BobUI::DBus)
    add_subdirectory(networkmanager)
    add_subdirectory(connman)
endif()

if(APPLE)
    add_subdirectory(apple)
endif()

if(ANDROID)
    add_subdirectory(android)
endif()

if(BOBUI_FEATURE_glib AND TARGET GLIB2::GOBJECT AND TARGET GLIB2::GIO)
    add_subdirectory(glib)
endif()
