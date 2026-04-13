# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qaccessibilitylinux LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

if (NOT BOBUI_FEATURE_accessibility OR NOT TARGET BobUI::LinuxAccessibilitySupportPrivate
        OR (BOBUI_BUILD_STANDALONE_TESTS AND BOBUI_WILL_INSTALL))
    return()
endif()

#####################################################################
## tst_qaccessibilitylinux Test:
#####################################################################

bobui_internal_add_test(tst_qaccessibilitylinux
    SOURCES
        tst_qaccessibilitylinux.cpp
    DBUS_INTERFACE_SOURCES
        ../../../../src/platformsupport/linuxaccessibility/dbusxml/Bus.xml
    LIBRARIES
        BobUI::DBus
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::LinuxAccessibilitySupportPrivate
        BobUI::Widgets
)

# require for struct_marshallers_p.h which is included dbus_interface.h
target_include_directories(tst_qaccessibilitylinux PRIVATE
    ../../../../src/platformsupport/linuxaccessibility
)
