# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

include(selfcover.cmake)

#####################################################################
## Test Module:
#####################################################################

include("${CMAKE_CURRENT_SOURCE_DIR}/BobUITestProperties.cmake")

bobui_internal_add_module(Test
    CONFIG_MODULE_NAME testlib
    QMAKE_MODULE_CONFIG console testlib_defines
    EXCEPTIONS
    SOURCES
        3rdparty/cycle/cycle_p.h # Needed by tests, so visible to syncbobui.
        3rdparty/cycle/cycle_include_p.h # Also needed by tests.
        3rdparty/catch2/catch_p.h # Also needed by tests.
        removed_api.cpp     # keep first
        qabstracttestlogger.cpp qabstracttestlogger_p.h
        qasciikey.cpp
        qbenchmark.cpp qbenchmark.h qbenchmark_p.h
        qbenchmarkevent.cpp qbenchmarkevent_p.h
        qbenchmarkmeasurement.cpp qbenchmarkmeasurement_p.h
        qbenchmarkmetric.cpp qbenchmarkmetric.h qbenchmarkmetric_p.h
        qbenchmarkperfevents.cpp qbenchmarkperfevents_p.h
        qbenchmarktimemeasurers_p.h
        qcomparisontesthelper.cpp qcomparisontesthelper_p.h
        qcsvbenchmarklogger.cpp qcsvbenchmarklogger_p.h
        qemulationdetector_p.h
        qjunittestlogger.cpp qjunittestlogger_p.h
        qplaintestlogger.cpp qplaintestlogger_p.h
        qpropertytesthelper_p.h
        qsignaldumper.cpp qsignaldumper_p.h
        qsignalspy.cpp qsignalspy.h
        bobuiaptestlogger.cpp bobuiaptestlogger_p.h
        bobuieamcitylogger.cpp bobuieamcitylogger_p.h
        bobuiest.h
        bobuiest_gui.h
        bobuiest_network.h
        bobuiest_widgets.h
        bobuiestaccessible.h
        bobuiestassert.h
        bobuiestblacklist.cpp bobuiestblacklist_p.h
        bobuiestcase.cpp bobuiestcase.h bobuiestcase_p.h
        bobuiestcoreelement_p.h
        bobuiestcrashhandler_p.h
        bobuiestdata.cpp bobuiestdata.h
        bobuiestelement.cpp bobuiestelement_p.h
        bobuiestelementattribute.cpp bobuiestelementattribute_p.h
        bobuiestevent.h
        bobuiesteventloop.h
        bobuiesthelpers_p.h
        bobuiestjunitstreamer.cpp bobuiestjunitstreamer_p.h
        bobuiestkeyboard.h
        bobuiestlog.cpp bobuiestlog_p.h
        bobuiestmouse.cpp bobuiestmouse.h
        bobuiestresult.cpp bobuiestresult_p.h
        bobuiestspontaneevent.h
        bobuiestsystem.h
        bobuiesttable.cpp bobuiesttable_p.h
        bobuiesttouch.h
        bobuiesttostring.h
        bobuiestwheel.h
        bobuitestglobal.h
        qxmltestlogger.cpp qxmltestlogger_p.h
    DEFINES
        BOBUI_NO_CAST_FROM_ASCII
        BOBUI_NO_CAST_TO_ASCII
        BOBUI_NO_CONTEXTLESS_CONNECT
        BOBUI_NO_DATASTREAM
        BOBUI_NO_FOREACH
        BOBUI_NO_URL_CAST_FROM_STRING
        BOBUI_USE_NODISCARD_FILE_OPEN
        # Ensure uniform location info between release and debug builds
        BOBUI_NO_MESSAGELOGCONTEXT
    LIBRARIES
        BobUI::CorePrivate
    NO_PCH_SOURCES
        removed_api.cpp
    PUBLIC_LIBRARIES
        BobUI::Core
    PRIVATE_MODULE_INTERFACE
        BobUI::CorePrivate
    ATTRIBUTION_FILE_DIR_PATHS
        3rdparty/catch2
        3rdparty/cycle
        3rdparty/linux
        3rdparty/valgrind
    EXTRA_CMAKE_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/BobUITestProperties.cmake"
    EXTRA_CMAKE_INCLUDES
        BobUITestProperties.cmake
)

if(TARGET Gui)
    set_property(TARGET Test
        APPEND PROPERTY _bobui_internal_sync_headers_deps Gui_sync_headers)
endif()

## Scopes:
#####################################################################

bobui_internal_extend_target(Test CONDITION BOBUI_FEATURE_gui
    SOURCES
        bobuiestaccessible.cpp
)

bobui_internal_extend_target(Test CONDITION BOBUI_FEATURE_itemmodeltester
    SOURCES
        qabstractitemmodeltester.cpp qabstractitemmodeltester.h
)

bobui_internal_extend_target(Test CONDITION BOBUI_FEATURE_batch_test_support
    SOURCES
        bobuiestregistry.cpp bobuiestregistry_p.h
)

# 3rdparty/valgrind/*_p.h deliberately omitted, so syncbobui doesn't
# mirror it in generated include/ directory - #include using "..._p.h"
# with a relative path. Only this module's sources and Core's qsimd.h
# should need to.
bobui_internal_extend_target(Test CONDITION BOBUI_FEATURE_valgrind
    SOURCES
        qbenchmarkvalgrind.cpp qbenchmarkvalgrind_p.h
)

bobui_internal_extend_target(Test CONDITION embedded
    COMPILE_OPTIONS
        -fno-rtti
)

bobui_internal_extend_target(Test CONDITION APPLE
    SOURCES
        qappletestlogger.cpp qappletestlogger_p.h
    PUBLIC_LIBRARIES
        ${FWSecurity}

    # cycle_p.h expects the configure system to set up these defines.
    # Pass through COMPILE_OPTIONS instead of DEFINES, as we know
    # Clang supports defines with function macros on the command line.
    COMPILE_OPTIONS
        "-DHAVE_ARMV8_CNTVCT_EL0='defined(Q_PROCESSOR_ARM)'"
)

bobui_internal_extend_target(Test CONDITION MACOS
    SOURCES
        bobuiestutil_macos.mm bobuiestutil_macos_p.h
    PUBLIC_LIBRARIES
        ${FWAppKit}
        ${FWApplicationServices}
        ${FWFoundation}
        ${FWIOKit}
)

bobui_internal_extend_target(Test CONDITION UNIX
    SOURCES
        bobuiestcrashhandler_unix.cpp
)

bobui_internal_extend_target(Test CONDITION WIN32
    SOURCES
        bobuiestcrashhandler_win.cpp
)

set(bobui_tc_build_dir "$<TARGET_PROPERTY:BOBUI_TESTCASE_BUILDDIR>")
set(bobui_bool_tc_build_dir "$<BOOL:${bobui_tc_build_dir}>")
set(bobui_tc_build_dir_def
    "$<IF:${bobui_bool_tc_build_dir},${bobui_tc_build_dir},$<TARGET_PROPERTY:BINARY_DIR>>"
)

string(JOIN "" testcase_dirs_genex
    "$<$<NOT:$<BOOL:$<TARGET_PROPERTY:BOBUI_SKIP_DEFAULT_TESTCASE_DIRS>>>:"
        "BOBUI_TESTCASE_BUILDDIR=\"${bobui_tc_build_dir_def}\"$<SEMICOLON>"
        "BOBUI_TESTCASE_SOURCEDIR=\"$<TARGET_PROPERTY:SOURCE_DIR>\""
    ">"
)
set_property(TARGET Test APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS
    ${testcase_dirs_genex}
)

bobui_internal_add_docs(Test
    doc/bobuitestlib.qdocconf
)

bobui_internal_apply_testlib_coverage_options(Test)

if(BOBUI_BUILD_DOC_SNIPPETS)
    add_subdirectory(doc/snippets/code)
endif()

if(WASM)
    # Keep in sync with is_test files in BobUI6WasmMacros.cmake
    set(testrunner_files
        "${BobUIBase_SOURCE_DIR}/util/wasm/batchedtestrunner/batchedtestrunner.html"
        "${BobUIBase_SOURCE_DIR}/util/wasm/batchedtestrunner/batchedtestrunner.js"
        "${BobUIBase_SOURCE_DIR}/util/wasm/batchedtestrunner/emrunadapter.js"
        "${BobUIBase_SOURCE_DIR}/util/wasm/batchedtestrunner/qwasmjsruntime.js"
        "${BobUIBase_SOURCE_DIR}/util/wasm/batchedtestrunner/qwasmtestmain.js"
        "${BobUIBase_SOURCE_DIR}/util/wasm/batchedtestrunner/util.js"
        "${BobUIBase_SOURCE_DIR}/util/wasm/batchedtestrunner/bobuiestoutputreporter.js"
        "${BobUIBase_SOURCE_DIR}/util/wasm/batchedtestrunner/bobuiestoutputreporter.css")

    bobui_path_join(install_dir_libexec "${BOBUI_INSTALL_DIR}" "${INSTALL_LIBEXECDIR}")
    bobui_copy_or_install(FILES ${testrunner_files}
            DESTINATION "${install_dir_libexec}")

    # In a prefix build, the above copy_or_install won't put the files
    # in the build dir, but they are required there for tests at configure time
    if(BOBUI_WILL_INSTALL)
        bobui_path_join(build_dir_libexec "${BOBUI_BUILD_DIR}" "${INSTALL_LIBEXECDIR}")
        foreach(testrunner_file ${testrunner_files})
            file(COPY "${testrunner_file}" DESTINATION "${build_dir_libexec}")
        endforeach()
    endif()
endif()
