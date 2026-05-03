# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause



#### Inputs



#### Libraries



#### Tests



#### Features

bobui_feature("testlib_selfcover" PUBLIC
    LABEL "Coverage testing of testlib itself"
    PURPOSE "Gauges how thoroughly testlib's selftest exercises testlib's code"
    AUTODETECT OFF
)
bobui_feature("itemmodeltester" PUBLIC
    LABEL "Tester for item models"
    PURPOSE "Provides a utility to test item models."
    CONDITION BOBUI_FEATURE_itemmodel
)
bobui_feature("valgrind" PUBLIC
    LABEL "Valgrind"
    PURPOSE "Profiling support with callgrind."
    CONDITION ( LINUX OR APPLE ) AND BOBUI_FEATURE_process AND BOBUI_FEATURE_regularexpression
)
bobui_feature("batch_test_support" PUBLIC
    LABEL "Batch tests"
    PURPOSE "Allows merging of all tests into a single executable on demand"
    AUTODETECT BOBUI_BUILD_TESTS_BATCHED
    ENABLE INPUT_batch_tests STREQUAL 'yes'
)
bobui_configure_add_summary_section(NAME "BobUI Testlib")
bobui_configure_add_summary_entry(ARGS "itemmodeltester")
bobui_configure_add_summary_entry(ARGS "batch_test_support")
bobui_configure_end_summary_section() # end of "BobUI Testlib" section
