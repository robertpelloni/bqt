
        if(NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6Test.lib"
                AND NOT QT_SBOM_BUILD_TIME AND NOT QT_SBOM_FAKE_CHECKSUM)
            if(NOT TRUE)
                message(FATAL_ERROR "Cannot find 'lib/Qt6Test.lib' to compute its checksum. "
                    "Expected to find it at '$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6Test.lib' ")
            endif()
        else()
            if(NOT QT_SBOM_BUILD_TIME)
                if(QT_SBOM_FAKE_CHECKSUM)
                    set(sha1 "158942a783ee1095eafacaffd93de73edeadbeef")
                else()
                    file(SHA1 "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6Test.lib" sha1)
                endif()

                set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-module-Test" true)

                list(APPEND QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES
                    "SPDXRef-Package-qtbase-qt-module-Test")
                list(APPEND
                    "QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES_SPDXRef-Package-qtbase-qt-module-Test"
                    "${sha1}")
            endif()
            file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
FileName: ./lib/Qt6Test.lib
SPDXID: SPDXRef-PackagedFile-qt-module-Test-Release
FileType: BINARY
FileChecksum: SHA1: ${sha1}
LicenseConcluded: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
FileCopyrightText: <text>Copyright (C) The Qt Company Ltd. and other contributors.</text>
LicenseInfoInFile: NOASSERTION
Relationship: SPDXRef-Package-qtbase-qt-module-Test CONTAINS SPDXRef-PackagedFile-qt-module-Test-Release
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/include/QtTest/qttestexports.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/3rdparty/cycle/cycle_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/3rdparty/cycle/cycle_include_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/3rdparty/catch2/catch_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/removed_api.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qabstracttestlogger.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qabstracttestlogger_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qasciikey.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qbenchmark.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qbenchmark.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qbenchmark_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qbenchmarkevent.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qbenchmarkevent_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qbenchmarkmeasurement.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qbenchmarkmeasurement_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qbenchmarkmetric.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qbenchmarkmetric.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qbenchmarkmetric_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qbenchmarkperfevents.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qbenchmarkperfevents_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qbenchmarktimemeasurers_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qcomparisontesthelper.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qcomparisontesthelper_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qcsvbenchmarklogger.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qcsvbenchmarklogger_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qemulationdetector_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qjunittestlogger.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qjunittestlogger_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qplaintestlogger.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qplaintestlogger_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qpropertytesthelper_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qsignaldumper.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qsignaldumper_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qsignalspy.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qsignalspy.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtaptestlogger.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtaptestlogger_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qteamcitylogger.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qteamcitylogger_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtest.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtest_gui.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtest_network.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtest_widgets.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtestaccessible.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtestassert.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtestblacklist.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtestblacklist_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtestcase.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtestcase.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtestcase_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtestcoreelement_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtestcrashhandler_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtestdata.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtestdata.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtestelement.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtestelement_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtestelementattribute.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtestelementattribute_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtestevent.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtesteventloop.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtesthelpers_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtestjunitstreamer.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtestjunitstreamer_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtestkeyboard.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtestlog.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtestlog_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtestmouse.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtestmouse.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtestresult.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtestresult_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtestspontaneevent.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtestsystem.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtesttable.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtesttable_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtesttouch.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtesttostring.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtestwheel.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qttestglobal.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qxmltestlogger.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qxmltestlogger_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qttestlib-config.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qttestlib-config_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/ddf_Test.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtestaccessible.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qabstractitemmodeltester.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qabstractitemmodeltester.h
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/qtestcrashhandler_win.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Test-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/testlib/doc/qttestlib.qdocconf
"
                )
        endif()
