# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT WASM)
    add_subdirectory(devicediscovery)
    add_subdirectory(fbconvenience)
endif()
if(BOBUI_FEATURE_evdev
    OR BOBUI_FEATURE_vxworksevdev
    OR BOBUI_FEATURE_integrityhid
    OR BOBUI_FEATURE_libinput
    OR BOBUI_FEATURE_tslib
    OR (BOBUI_FEATURE_libinput AND BOBUI_FEATURE_xkbcommon))
    add_subdirectory(input)
endif()
if(BOBUI_FEATURE_kms)
    add_subdirectory(kmsconvenience)
endif()
