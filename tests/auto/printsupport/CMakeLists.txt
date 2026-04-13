# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_FEATURE_printer OR NOT BOBUI_FEATURE_printdialog)
    return()
endif()
add_subdirectory(dialogs)
add_subdirectory(kernel)
