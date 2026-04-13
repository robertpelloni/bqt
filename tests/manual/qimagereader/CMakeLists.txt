# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## qimagereader Binary:
#####################################################################

bobui_internal_add_manual_test(qimagereader
    GUI
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
)
set(qimagereader_resource_files
    "BobUI_logostrap_CMYK.jpg"
)
bobui_add_resources(qimagereader "qimagereader"
    PREFIX
        "/"
    FILES
        ${qimagereader_resource_files}
)
