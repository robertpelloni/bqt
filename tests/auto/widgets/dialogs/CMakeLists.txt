# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

add_subdirectory(qcolordialog)
add_subdirectory(qdialog)
add_subdirectory(qerrormessage)
add_subdirectory(qfiledialog2)
add_subdirectory(qfontdialog)
add_subdirectory(qinputdialog)
add_subdirectory(qprogressdialog)
add_subdirectory(qwizard)
add_subdirectory(qfiledialog)
# BOBUIBUG-101217, qmessagebox hangs on Android
if(NOT ANDROID)
    add_subdirectory(qmessagebox)
endif()
if(BOBUI_FEATURE_private_tests)
    add_subdirectory(qsidebar)
endif()
