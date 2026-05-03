# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(BOBUI_FEATURE_xkbcommon)
    add_subdirectory(compose)
endif()
if(BOBUI_FEATURE_xkbcommon AND TARGET BobUI::DBus AND UNIX AND NOT MACOS)
    add_subdirectory(ibus)
endif()
