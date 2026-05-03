# Copyright (C) 2023 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_manual_qdatastream Test:
#####################################################################

bobui_internal_add_manual_test(tst_manual_qdatastream
    SOURCES
        tst_manualqdatastream.cpp
    LIBRARIES
        BobUI::Core
    ENABLE_AUTOGEN_TOOLS
        uic
)
