# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# The whole file is written manually.

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_selftests LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

include(../../../../src/testlib/selfcover.cmake)

# ------------- Test runner -------------

# Resources:

file(GLOB qmake_expected_files_resource_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "expected_*")
foreach(file IN LISTS qmake_expected_files_resource_files)
    set_source_files_properties("${CMAKE_CURRENT_SOURCE_DIR}/${file}" PROPERTIES BOBUI_RESOURCE_ALIAS "${file}")
endforeach()

bobui_internal_add_test(tst_selftests
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/"
    EXCEPTIONS
    CATCH
    SOURCES
        tst_selftests.cpp
        catch.cpp
    NO_PCH_SOURCES
        tst_selftests.cpp # undef BOBUI_NO_FOREACH
    LIBRARIES
        BobUI::TestPrivate
    TESTDATA ${qmake_expected_files_resource_files}
    BUILTIN_TESTDATA
    # These lines need to be commented out as they need to be enabled
    # COMPILE_OPTIONS
    #   conditionally
    #   --cs-exclude-file-abs-wildcard=${BOBUI_SOURCE_TREE}/*
    #   --cs-include-file-abs-wildcard=*/src/testlib/*
    #   --cs-mcc
    #   --cs-mcdc
    # LINK_OPTIONS
    #   "$$COVERAGE_OPTIONS"
)

bobui_internal_apply_testlib_coverage_options(tst_selftests)

# ------------- Sub tests -------------

set(subprograms
    assert
    badxml
    benchlibcounting
    benchlibeventcounter
    benchliboptions
    benchlibtickcounter
    benchlibwalltime
    blacklisted
    cmptest
    commandlinedata
    counting
    crashes
    datatable
    datetime
    deleteLater
    deleteLater_noApp
    differentexec
    eventloop
    exceptionthrow
    expectfail
    extendedcompare
    failcleanup
    failcleanuptestcase
    faildatatype
    failfetchtype
    failinit
    failinitdata
    fetchbogus
    findtestdata
    float
    globaldata
    junit
    longstring
    maxwarnings
    multiexec
    pass
    pairdiagnostics
    printdatatags
    printdatatagswithglobaltags
    qexecstringlist
    silent
    silent_fatal
    signaldumper
    singleskip
    skip
    skipblacklisted
    skipcleanup
    skipcleanuptestcase
    skipinit
    skipinitdata
    sleep
    strcmp
    subtest
    testlib
    tuplediagnostics
    verbose1
    verbose2
    verifyexceptionthrown
    warnings
    watchdog
)

if(NOT BOBUI_FEATURE_sanitize_address)
    list(APPEND subprograms
        # valgrind and asan are incompatible:
        #   "ASan runtime does not come first in initial library list;"
        benchlibcallgrind
    )
endif()

if(FEATURE_concurrent AND FEATURE_future)
    list(APPEND subprograms
        # This test needs exceptions, QException, and BobUIConcurrent::run()
        throwonfailandskip
    )
endif()

if(FEATURE_cxx20)
  list(APPEND subprograms
      threewaycompare
  )
endif()

if(TARGET BobUI::Gui)
    list(APPEND subprograms
        keyboard
        mouse
    )
endif()

# Ensure uniform location info between release and debug builds
add_compile_definitions(BOBUI_MESSAGELOGCONTEXT)

foreach(subprogram IN LISTS subprograms)
    add_subdirectory(${subprogram})
    if(BOBUI_FEATURE_process)
        add_dependencies(tst_selftests ${subprogram})
    endif()
endforeach()

list(JOIN subprograms " " subprograms)
bobui_internal_extend_target(tst_selftests
    DEFINES
        $<$<COMPILE_LANGUAGE:CXX>:SUBPROGRAMS=${subprograms}>
)
