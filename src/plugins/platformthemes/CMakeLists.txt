# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause


if(BOBUI_FEATURE_dbus AND BOBUI_FEATURE_mimetype AND BOBUI_FEATURE_regularexpression AND UNIX AND NOT APPLE)
    add_subdirectory(xdgdesktopportal)
endif()
if(BOBUI_FEATURE_gtk3 AND TARGET BobUI::Widgets)
    add_subdirectory(gtk3)
endif()
