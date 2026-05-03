# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bobuiextstream Test:
#####################################################################

# Collect test data
list(APPEND test_data "../rfc3261.txt")
list(APPEND test_data "../task113817.txt")
list(APPEND test_data "../bobuiextstream.qrc")
list(APPEND test_data "../bobuiextstream_integrity.qrc")
list(APPEND test_data "../tst_bobuiextstream.cpp")
list(APPEND test_data "../resources")
list(APPEND test_data "../BLACKLIST")

bobui_internal_add_test(tst_bobuiextstream
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../"
    SOURCES
        ../tst_bobuiextstream.cpp
    LIBRARIES
        BobUI::Network
        BobUI::TestPrivate
    TESTDATA ${test_data}
    BOBUI_TEST_SERVER_LIST "cyrus"
)

if(BOBUI_FEATURE_sanitize_address)
    set_property(TEST tst_bobuiextstream APPEND PROPERTY ENVIRONMENT "BOBUIEST_FUNCTION_TIMEOUT=900000")
endif()

# Resources:
set(bobuiextstream_resource_files
    "../resources/big_endian/"
    "../resources/little_endian/"
)

bobui_internal_add_resource(tst_bobuiextstream "bobuiextstream"
    PREFIX
        "/tst_textstream/"
    BASE
        ".."
    FILES
        ${bobuiextstream_resource_files}
)

if(BOBUI_FEATURE_process)
    add_dependencies(tst_bobuiextstream stdinProcess readAllStdinProcess readLineStdinProcess)
endif()
