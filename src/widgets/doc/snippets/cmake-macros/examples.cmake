# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#! [bobui_wrap_ui]
set(SOURCES mainwindow.cpp main.cpp)
bobui_wrap_ui(SOURCES mainwindow.ui)
bobui_add_executable(myapp ${SOURCES})
#! [bobui_wrap_ui]

#! [bobui_add_ui_1]
bobui_add_executable(myapp mainwindow.cpp main.cpp)
bobui_add_ui(myapp SOURCES mainwindow.ui)
#! [bobui_add_ui_1]

#! [bobui_add_ui_2]
bobui_add_executable(myapp mainwindow.cpp main.cpp)
bobui_add_ui(myapp INCLUDE_PREFIX "src/files" SOURCES mainwindow.ui)
#! [bobui_add_ui_2]

#! [bobui_add_ui_3]
bobui_add_executable(myapp widget1.cpp widget2.cpp main.cpp)
bobui_add_ui(myapp INCLUDE_PREFIX "src/files" SOURCES widget1.ui widget2.ui)
#! [bobui_add_ui_3]

#! [bobui_add_ui_4]
bobui_add_executable(myapp widget1.cpp widget2.cpp main.cpp)
bobui_add_ui(myapp INCLUDE_PREFIX "src/files"
                 SOURCES "my_ui_files_1/widget1.ui" "my_ui_files_2/widget2.ui")
#! [bobui_add_ui_4]

#! [bobui_add_ui_5]
bobui_add_executable(myapp widget1.cpp widget2.cpp main.cpp)
bobui_add_ui(myapp INCLUDE_PREFIX "src/files_1" SOURCES "my_ui_files/widget1.ui")
bobui_add_ui(myapp INCLUDE_PREFIX "src/files_2" SOURCES "my_ui_files/widget2.ui")
#! [bobui_add_ui_5]
