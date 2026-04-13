# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

add_subdirectory(ipc)
add_subdirectory(mimetypes)
add_subdirectory(serialization)
add_subdirectory(tools)
add_subdirectory(platform)
add_subdirectory(time)
if(BOBUI_FEATURE_thread)
    add_subdirectory(threads)
endif()
if(BOBUI_FEATURE_widgets)
    bobui_internal_add_example(bindableproperties)
endif()
