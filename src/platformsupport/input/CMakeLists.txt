# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_find_package(Libinput MODULE)
bobui_find_package(XKB MODULE)
bobui_find_package(Tslib MODULE)
bobui_find_package(Mtdev MODULE)

#####################################################################
## InputSupportPrivate Module:
#####################################################################

bobui_internal_add_module(InputSupportPrivate
    CONFIG_MODULE_NAME input_support
    STATIC
    INTERNAL_MODULE
    DEFINES
        BOBUI_NO_CAST_FROM_ASCII
    PUBLIC_LIBRARIES
        BobUI::CorePrivate
        BobUI::DeviceDiscoverySupportPrivate
        BobUI::GuiPrivate
    PRECOMPILED_HEADER
        "../../corelib/global/bobui_pch.h"
    NO_GENERATE_CPP_EXPORTS
)

## Scopes:
#####################################################################

bobui_internal_extend_target(InputSupportPrivate CONDITION BOBUI_FEATURE_evdev
    SOURCES
        evdevkeyboard/qevdevkeyboard_defaultmap_p.h
        evdevkeyboard/qevdevkeyboardhandler.cpp evdevkeyboard/qevdevkeyboardhandler_p.h
        evdevkeyboard/qevdevkeyboardmanager.cpp evdevkeyboard/qevdevkeyboardmanager_p.h
        evdevmouse/qevdevmousehandler.cpp evdevmouse/qevdevmousehandler_p.h
        evdevmouse/qevdevmousemanager.cpp evdevmouse/qevdevmousemanager_p.h
        evdevtouch/qevdevtouchhandler.cpp evdevtouch/qevdevtouchhandler_p.h
        evdevtouch/qevdevtouchmanager.cpp evdevtouch/qevdevtouchmanager_p.h
    INCLUDE_DIRECTORIES
        evdevtouch/../shared
)

bobui_internal_extend_target(InputSupportPrivate CONDITION BOBUI_FEATURE_evdev AND BOBUI_FEATURE_tabletevent
    SOURCES
        evdevtablet/qevdevtablethandler.cpp evdevtablet/qevdevtablethandler_p.h
        evdevtablet/qevdevtabletmanager.cpp evdevtablet/qevdevtabletmanager_p.h
)

bobui_internal_extend_target(InputSupportPrivate CONDITION BOBUI_FEATURE_evdev AND BOBUI_FEATURE_libudev AND BOBUI_FEATURE_tabletevent
    LIBRARIES
        PkgConfig::Libudev
)

bobui_internal_extend_target(InputSupportPrivate CONDITION BOBUI_FEATURE_evdev AND BOBUI_FEATURE_libudev
    LIBRARIES
        PkgConfig::Libudev
)

bobui_internal_extend_target(InputSupportPrivate CONDITION BOBUI_FEATURE_evdev AND BOBUI_FEATURE_mtdev
    LIBRARIES
        PkgConfig::Mtdev
)

bobui_internal_extend_target(InputSupportPrivate CONDITION BOBUI_FEATURE_vxworksevdev
    SOURCES
        vxkeyboard/qvxkeyboard_defaultmap_p.h
        vxkeyboard/qvxkeyboardhandler.cpp vxkeyboard/qvxkeyboardhandler_p.h
        vxkeyboard/qvxkeyboardmanager.cpp vxkeyboard/qvxkeyboardmanager_p.h
        vxmouse/qvxmousehandler.cpp vxmouse/qvxmousehandler_p.h
        vxmouse/qvxmousemanager.cpp vxmouse/qvxmousemanager_p.h
        vxtouch/qvxtouchhandler.cpp vxtouch/qvxtouchhandler_p.h
        vxtouch/qvxtouchmanager.cpp vxtouch/qvxtouchmanager_p.h
    INCLUDE_DIRECTORIES
        shared
)

bobui_internal_extend_target(InputSupportPrivate CONDITION BOBUI_FEATURE_tslib
    SOURCES
        tslib/bobuislib.cpp tslib/bobuislib_p.h
    PUBLIC_LIBRARIES
        PkgConfig::Tslib
)

bobui_internal_extend_target(InputSupportPrivate CONDITION BOBUI_FEATURE_libinput
    SOURCES
        libinput/qlibinputhandler.cpp libinput/qlibinputhandler_p.h
        libinput/qlibinputkeyboard.cpp libinput/qlibinputkeyboard_p.h
        libinput/qlibinputpointer.cpp libinput/qlibinputpointer_p.h
        libinput/qlibinputtouch.cpp libinput/qlibinputtouch_p.h
    INCLUDE_DIRECTORIES
        libinput/../shared
    LIBRARIES
        Libinput::Libinput
        PkgConfig::Libudev
)

bobui_internal_extend_target(InputSupportPrivate CONDITION BOBUI_FEATURE_libinput AND BOBUI_FEATURE_xkbcommon
    LIBRARIES
        XKB::XKB
)

bobui_internal_extend_target(InputSupportPrivate CONDITION BOBUI_FEATURE_evdev OR BOBUI_FEATURE_libinput OR BOBUI_FEATURE_vxworksevdev
    SOURCES
        shared/devicehandlerlist_p.h
        shared/qevdevutil.cpp shared/qevdevutil_p.h
        shared/qfdcontainer.cpp shared/qfdcontainer_p.h
        shared/qkeyboardmap_p.h
        shared/qkeycodeaction_p.h
        shared/qoutputmapping.cpp shared/qoutputmapping_p.h
        shared/bobuiouchfilter_p.h
)

bobui_internal_extend_target(InputSupportPrivate CONDITION BOBUI_FEATURE_integrityhid
    SOURCES
        integrityhid/qintegrityhidmanager.cpp integrityhid/qintegrityhidmanager.h
    LIBRARIES
        integrityhid
)
