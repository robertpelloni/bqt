# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

if(NOT TARGET BobUI6::Widgets)
    return()
endif()
if(BOBUI_FEATURE_sharedmemory AND BOBUI_FEATURE_systemsemaphore)
    bobui_internal_add_example(sharedmemory)
endif()
if(BOBUI_FEATURE_localserver AND TARGET BobUI6::Network)
    bobui_internal_add_example(localfortuneserver)
    bobui_internal_add_example(localfortuneclient)
endif()
