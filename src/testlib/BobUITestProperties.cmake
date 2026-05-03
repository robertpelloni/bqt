# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# This file contains the property definitions that are known by BobUI Test

if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.23")
    set(bobui_skip_default_testcase_dirs_extra_agrs
        INITIALIZE_FROM_VARIABLE
        BOBUI_SKIP_DEFAULT_TESTCASE_DIRS
    )
elseif(DEFINED BOBUI_SKIP_DEFAULT_TESTCASE_DIRS)
    message(WARNING "BOBUI_SKIP_DEFAULT_TESTCASE_DIRS is set to ${BOBUI_SKIP_DEFAULT_TESTCASE_DIRS},"
        " but the variable is not supported by this CMake version. Please set the"
        " BOBUI_SKIP_DEFAULT_TESTCASE_DIRS target property where is required.")
endif()

define_property(TARGET
    PROPERTY
        BOBUI_SKIP_DEFAULT_TESTCASE_DIRS
    BRIEF_DOCS
        "Disables the test case directory definitions for the BobUI Test targets."
    FULL_DOCS
        "By default the definitions BOBUI_TESTCASE_SOURCEDIR and BOBUI_TESTCASE_BUILDDIR point to the
        target source and build directories of the target accordingly. If
        BOBUI_SKIP_DEFAULT_TESTCASE_DIRS is set to TRUE the macros remain empty."
    ${bobui_skip_default_testcase_dirs_extra_agrs}
)
