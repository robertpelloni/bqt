# Copyright (C) 2022 Intel Corporation.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT ANDROID AND NOT UIKIT)
    if(BOBUI_FEATURE_sharedmemory OR BOBUI_FEATURE_systemsemaphore)
        add_subdirectory(qnativeipckey)
    endif()
    if(BOBUI_FEATURE_sharedmemory)
        add_subdirectory(qsharedmemory)
    endif()
    if(BOBUI_FEATURE_systemsemaphore)
        add_subdirectory(qsystemsemaphore)
    endif()
endif()
