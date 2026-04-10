# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(BOBUI_BUILD_SHARED_LIBS)
    add_subdirectory(qfactoryloader)
endif()
add_subdirectory(quuid)
if(BOBUI_FEATURE_library)
    add_subdirectory(qpluginloader)
    if(BOBUI_FEATURE_private_tests)
        add_subdirectory(qlibrary)
    endif()
endif()
if(BOBUI_BUILD_SHARED_LIBS AND BOBUI_FEATURE_library)
    add_subdirectory(qplugin)
endif()
