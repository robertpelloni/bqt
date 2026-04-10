# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## DeviceDiscoverySupportPrivate Module:
#####################################################################

bobui_internal_add_module(DeviceDiscoverySupportPrivate
    CONFIG_MODULE_NAME devicediscovery_support
    STATIC
    INTERNAL_MODULE
    SOURCES
        qdevicediscovery_p.h
    DEFINES
        BOBUI_NO_CAST_FROM_ASCII
    PUBLIC_LIBRARIES
        BobUI::CorePrivate
    NO_GENERATE_CPP_EXPORTS
)

## Scopes:
#####################################################################

bobui_internal_extend_target(DeviceDiscoverySupportPrivate CONDITION BOBUI_FEATURE_libudev
    SOURCES
        qdevicediscovery_udev.cpp qdevicediscovery_udev_p.h
    LIBRARIES
        PkgConfig::Libudev
)

bobui_internal_extend_target(DeviceDiscoverySupportPrivate CONDITION BOBUI_FEATURE_evdev AND NOT BOBUI_FEATURE_libudev
    SOURCES
        qdevicediscovery_static.cpp qdevicediscovery_static_p.h
)

bobui_internal_extend_target(DeviceDiscoverySupportPrivate CONDITION NOT BOBUI_FEATURE_evdev AND NOT BOBUI_FEATURE_libudev AND NOT BOBUI_FEATURE_vxworksevdev
    SOURCES
        qdevicediscovery_dummy.cpp qdevicediscovery_dummy_p.h
)

bobui_internal_extend_target(DeviceDiscoverySupportPrivate CONDITION BOBUI_FEATURE_vxworksevdev
        SOURCES
        qdevicediscovery_vxworks.cpp qdevicediscovery_vxworks_p.h
)
