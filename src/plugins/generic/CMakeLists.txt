# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(BOBUI_FEATURE_evdev)
    add_subdirectory(evdevmouse)
    add_subdirectory(evdevtouch)
    add_subdirectory(evdevkeyboard)
endif()
if(BOBUI_FEATURE_evdev AND BOBUI_FEATURE_tabletevent)
    add_subdirectory(evdevtablet)
endif()
if(BOBUI_FEATURE_vxworksevdev)
    add_subdirectory(vxkeyboard)
    add_subdirectory(vxmouse)
    add_subdirectory(vxtouch)
endif()
if(BOBUI_FEATURE_tslib)
    add_subdirectory(tslib)
endif()
if(BOBUI_FEATURE_tuiotouch)
    add_subdirectory(tuiotouch)
endif()
if(BOBUI_FEATURE_libinput)
    add_subdirectory(libinput)
endif()
if(FREEBSD)
    # add_subdirectory(bsdkeyboard) # TODO: BOBUIBUG-112770
    # add_subdirectory(bsdmouse) # TODO: BOBUIBUG-112770
endif()
