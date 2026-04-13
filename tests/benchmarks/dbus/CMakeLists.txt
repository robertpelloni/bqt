# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

add_subdirectory(qdbustype)
if(BOBUI_FEATURE_process)
    add_subdirectory(qdbusperformance)
endif()
