# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(TARGET BobUI::Gui AND TARGET BobUI::Network AND BOBUI_FEATURE_pdf)
    add_subdirectory(painting)
endif()
if(TARGET BobUI::Network AND TARGET BobUI::Widgets)
    add_subdirectory(widgets)
    add_subdirectory(stylesheet)
    add_subdirectory(text)
    add_subdirectory(styles)
endif()
