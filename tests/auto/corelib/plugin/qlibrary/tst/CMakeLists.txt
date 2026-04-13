# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qlibrary Test:
#####################################################################

# Collect test data
list(APPEND test_data "../library_path/invalid.so")

bobui_internal_add_test(tst_qlibrary
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../"
    SOURCES
        ../tst_qlibrary.cpp
    TESTDATA ${test_data}
    LIBRARIES mylib mylib2 BobUI::CorePrivate
)

add_dependencies(tst_qlibrary mylib mylib2)

if(ANDROID)
    list(APPEND extra_libs
        "${CMAKE_CURRENT_BINARY_DIR}/../libmylib.so")
    list(APPEND extra_libs
        "${CMAKE_CURRENT_BINARY_DIR}/../libsystem.bobui.test.mylib.so")
    set_target_properties(tst_qlibrary PROPERTIES
        BOBUI_ANDROID_EXTRA_LIBS "${extra_libs}")
endif()
