# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

add_subdirectory(qstandarditem)
if(TARGET BobUI::Widgets)
    add_subdirectory(qstandarditemmodel)
endif()
if(TARGET BobUI::Widgets AND NOT MINGW)
    add_subdirectory(qfilesystemmodel)
endif()
