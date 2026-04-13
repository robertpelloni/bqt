# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qprocess Test:
#####################################################################

bobui_internal_add_test(tst_qprocess
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../"
    SOURCES
        ../tst_qprocess.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Network
        BobUI::TestPrivate
)

## Build assorted sub-programs called from the test:
add_dependencies(tst_qprocess
    testProcessCrash
    testProcessEcho
    testProcessEcho2
    testProcessEcho3
    testProcessEnvironment
    testProcessGrandchild
    testProcessHang
    testProcessNormal
    testProcessOutput
    testProcessDeadWhileReading
    testProcessEOF
    testExitCodes
    testForwarding
    testDetached
    fileWriterProcess
    testSetWorkingDirectory
    testSoftExit
    nospace onespace twospaces
    testSpaceInName
)
if(TARGET BobUI::Widgets)
    add_dependencies(tst_qprocess testGuiProcess)
endif()
if(WIN32)
    add_dependencies(tst_qprocess testProcessEchoGui testSetNamedPipeHandleState)
endif()

if(UNIX)
    add_dependencies(tst_qprocess testUnixProcessParameters)
endif()
