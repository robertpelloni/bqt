# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

if(NOT ANDROID)
    bobui_internal_add_example(cbordump)
    bobui_internal_add_example(convert)
    bobui_internal_add_example(savegame)
endif()
if(TARGET BobUI6::Widgets)
    bobui_internal_add_example(streambookmarks)
endif()
