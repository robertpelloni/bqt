# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

add_subdirectory(initmain)
add_subdirectory(outformat)
add_subdirectory(qsignalspy)
add_subdirectory(tostring)

# BOBUIBUG-88507
if(BOBUI_FEATURE_process AND NOT ANDROID)
    add_subdirectory(selftests)
endif()
if(TARGET BobUI::Widgets)
    add_subdirectory(qabstractitemmodeltester)
endif()
