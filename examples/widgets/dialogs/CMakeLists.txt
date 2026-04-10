# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

if(BOBUI_FEATURE_wizard)
    bobui_internal_add_example(trivialwizard)
    if(BobUI6::PrintSupport)
        bobui_internal_add_example(licensewizard)
    endif()
endif()
bobui_internal_add_example(standarddialogs)
bobui_internal_add_example(tabdialog)
