# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qlocaledata Test:
#####################################################################

bobui_internal_add_test(tst_qlocaledata
    SOURCES
        tst_qlocaledata.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::TestPrivate
)
