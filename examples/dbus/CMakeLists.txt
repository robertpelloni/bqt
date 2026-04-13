# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

if(NOT TARGET BobUI6::DBus)
    return()
endif()
if(NOT ANDROID)
    bobui_internal_add_example(pingpong)
    if(BOBUI_FEATURE_process)
        bobui_internal_add_example(complexpingpong)
    endif()
endif()
if(TARGET BobUI6::Widgets)
    bobui_internal_add_example(chat)
    bobui_internal_add_example(remotecontrolledcar)
endif()
