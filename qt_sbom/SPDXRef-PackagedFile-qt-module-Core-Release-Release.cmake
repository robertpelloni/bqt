
        if(NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6Core.lib"
                AND NOT QT_SBOM_BUILD_TIME AND NOT QT_SBOM_FAKE_CHECKSUM)
            if(NOT TRUE)
                message(FATAL_ERROR "Cannot find 'lib/Qt6Core.lib' to compute its checksum. "
                    "Expected to find it at '$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6Core.lib' ")
            endif()
        else()
            if(NOT QT_SBOM_BUILD_TIME)
                if(QT_SBOM_FAKE_CHECKSUM)
                    set(sha1 "158942a783ee1095eafacaffd93de73edeadbeef")
                else()
                    file(SHA1 "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6Core.lib" sha1)
                endif()

                set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-module-Core" true)

                list(APPEND QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES
                    "SPDXRef-Package-qtbase-qt-module-Core")
                list(APPEND
                    "QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES_SPDXRef-Package-qtbase-qt-module-Core"
                    "${sha1}")
            endif()
            file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
FileName: ./lib/Qt6Core.lib
SPDXID: SPDXRef-PackagedFile-qt-module-Core-Release
FileType: BINARY
FileChecksum: SHA1: ${sha1}
LicenseConcluded: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
FileCopyrightText: <text>Copyright (C) The Qt Company Ltd. and other contributors.</text>
LicenseInfoInFile: NOASSERTION
Relationship: SPDXRef-Package-qtbase-qt-module-Core CONTAINS SPDXRef-PackagedFile-qt-module-Core-Release
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/include/QtCore/qtcoreexports.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qsimd.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qsimd.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qsimd_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qhash.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qhash.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/compat/removed_api.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/archdetect.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qalloc.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qalloc.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qassert.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qassert.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qcheckedint_impl.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qcompare_impl.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qcompare.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qcompare.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qcomparehelpers.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qcompilerdetection.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qconstructormacros.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qcontainerinfo.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qdarwinhelpers.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qendian.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qendian.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qendian_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qexceptionhandling.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qexpected_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qflags.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qfloat16.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qfloat16.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qforeach.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qfunctionpointer.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qgettid_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qglobal.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qglobal.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qglobal_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qglobalstatic.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qhooks.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qhooks_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qlibraryinfo.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qlibraryinfo.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qlibraryinfo_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qlogging.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qlogging.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qlogging_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qmalloc.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qmalloc.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qminmax.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qnamespace.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qnativeinterface.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qnativeinterface_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qnumeric.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qnumeric.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qnumeric_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qoperatingsystemversion.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qoperatingsystemversion.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qoperatingsystemversion_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qoverload.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qprocessordetection.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qrandom.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qrandom.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qrandom_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qstdlibdetection.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qswap.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qsysinfo.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qsysinfo.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qsystemdetection.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qtclasshelpermacros.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qtclasshelper_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qtconfiginclude.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qtconfigmacros.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qtcoreglobal.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qtcoreglobal_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qtdeprecationmarkers.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qtenvironmentvariables.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qtenvironmentvariables.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qtenvironmentvariables_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qtnoop.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qtpreprocessorsupport.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qtrace_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qtresource.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qtsymbolmacros.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qttranslation.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qttypetraits.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qtversionchecks.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qtversion.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qtypeinfo.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qtypes.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qtypes.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qvolatile_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/q17memory.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/q20algorithm.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/q20bit.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/q20chrono.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/q20functional.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/q20iterator.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/q20map.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/q20memory.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/q20type_traits.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/q20utility.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/q20vector.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/q23functional.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/q23utility.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/q23type_traits.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/q23utility.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/q26numeric.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qxpfunctional.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qxptype_traits.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qversiontagging.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/ipc/qsharedmemory.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/ipc/qsharedmemory.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/ipc/qsharedmemory_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/ipc/qsystemsemaphore.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/ipc/qsystemsemaphore.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/ipc/qsystemsemaphore_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/ipc/qtipccommon.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/ipc/qtipccommon.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/ipc/qtipccommon_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qabstractfileengine.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qabstractfileengine_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qbuffer.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qbuffer.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qdataurl.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qdataurl_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qdebug.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qdebug.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qdebug_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qdir.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qdir.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qdir_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qdirlisting.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qdirlisting.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qdirentryinfo_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qdiriterator.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qdiriterator.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qfile.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qfile.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qfile_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qfiledevice.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qfiledevice.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qfiledevice_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qfileinfo.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qfileinfo.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qfileinfo_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qfileselector.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qfileselector.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qfileselector_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qfilesystemengine.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qfilesystemengine_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qfilesystementry.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qfilesystementry_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qfilesystemiterator_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qfilesystemmetadata_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qfsfileengine.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qfsfileengine_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qfsfileengine_iterator.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qfsfileengine_iterator_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qiodevice.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qiodevice.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qiodevice_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qiodevicebase.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qipaddress.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qipaddress_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qlockfile.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qlockfile.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qlockfile_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qloggingcategory.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qloggingcategory.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qloggingcategory_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qloggingregistry.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qloggingregistry_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qnoncontiguousbytedevice.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qnoncontiguousbytedevice_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qresource.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qresource.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qresource_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qresource_iterator.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qresource_iterator_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qsavefile.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qsavefile.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qsavefile_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qstandardpaths.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qstandardpaths.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qstorageinfo.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qstorageinfo.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qstorageinfo_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qtemporarydir.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qtemporarydir.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qtemporaryfile.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qtemporaryfile.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qtemporaryfile_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qurl.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qurl.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qurl_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qurlidna.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qurlquery.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qurlquery.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qurlrecode.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qzipreader_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qzipwriter_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qzip.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/wcharhelpers_win_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qabstracteventdispatcher.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qabstracteventdispatcher.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qabstracteventdispatcher_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qabstractnativeeventfilter.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qabstractnativeeventfilter.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qapplicationstatic.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qassociativeiterable.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qassociativeiterable.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qbasictimer.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qbasictimer.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qbindingstorage.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qchronotimer.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qchronotimer.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qcoreapplication.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qcoreapplication.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qcoreapplication_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qcoreapplication_platform.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qcoreevent.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qcoreevent.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qcoreevent_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qdeadlinetimer.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qdeadlinetimer.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qelapsedtimer.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qelapsedtimer.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qeventloop.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qeventloop.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qeventloop_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qfunctions_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qiterable.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qiterable.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qiterable_impl.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qmath.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qmath.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qmetaassociation.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qmetaassociation.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qmetacontainer.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qmetacontainer.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qmetaobject.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qmetaobject.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qmetaobject_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qmetaobject_moc_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qmetaobjectbuilder.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qmetaobjectbuilder_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qmetasequence.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qmetasequence.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qmetatype.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qmetatype.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qmetatype_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qmimedata.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qmimedata.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qtmetamacros.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qtmocconstants.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qtmochelpers.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qobject.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qobject.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qobject_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qobject_p_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qobject_impl.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qobjectcleanuphandler.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qobjectcleanuphandler.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qobjectdefs.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qobjectdefs_impl.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qpointer.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qproperty.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qproperty.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qproperty_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qpropertyprivate.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qsequentialiterable.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qsequentialiterable.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qsignalmapper.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qsignalmapper.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qsingleshottimer.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qsingleshottimer_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qsocketnotifier.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qsocketnotifier.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qsystemerror.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qsystemerror_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qtestsupport_core.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qtestsupport_core.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qtimer.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qtimer.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qtimer_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qtranslator.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qtranslator.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qtranslator_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qvariant.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qvariant.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qvariant_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qvariantmap.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qvarianthash.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qvariantlist.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/plugin/qfactoryinterface.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/plugin/qfactoryinterface.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/plugin/qfactoryloader.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/plugin/qfactoryloader_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/plugin/qplugin.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/plugin/qplugin_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/plugin/qpluginloader.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/plugin/qpluginloader.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/plugin/quuid.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/plugin/quuid.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/plugin/quuid_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qcborarray.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qcborcommon.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qcborcommon.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qcborcommon_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qcbordiagnostic.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qcbormap.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qcborstream.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qcborvalue.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qcborvalue.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qcborvalue_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qdatastream.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qdatastream.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qjson_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qjsonarray.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qjsonarray.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qjsoncbor.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qjsondocument.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qjsondocument.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qjsonobject.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qjsonobject.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qjsonparseerror.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qjsonparser.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qjsonparser_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qjsonvalue.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qjsonvalue.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qjsonwriter.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qjsonwriter_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qtextstream.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qtextstream.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qtextstream_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qxmlutils.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qxmlutils_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qanystringview.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qanystringview.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qbytearray.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qbytearray.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qbytearrayalgorithms.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qbytearraylist.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qbytearraylist.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qbytearraymatcher.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qbytearraymatcher.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qbytearrayview.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qchar.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qcollator.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qcollator.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qcollator_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qdoublescanprint_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qlatin1stringmatcher.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qlatin1stringmatcher.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qlatin1stringview.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qlocale.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qlocale.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qlocale_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qlocale_tools.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qlocale_tools_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qstaticlatin1stringmatcher.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qstring.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qstring.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qstringalgorithms.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qstringalgorithms_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qstringbuilder.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qstringbuilder.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qstringconverter_base.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qstringconverter.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qstringconverter.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qstringconverter_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qstringfwd.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qstringiterator_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qstringlist.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qstringlist.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qstringliteral.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qstringmatcher.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qstringtokenizer.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qstringtokenizer.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qstringview.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qstringview.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qtextboundaryfinder.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qtextboundaryfinder.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qtformat_impl.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qunicodetables_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qunicodetools.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qunicodetools_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qutf8stringview.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qvsnprintf.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qatomic.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qatomic_cxx11.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qatomicwait.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qatomicwait_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qbasicatomic.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qgenericatomic.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qlocking_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qmutex.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qorderedmutexlocker_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qreadwritelock.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qrunnable.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qrunnable.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qthread.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qthread.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qthread_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qthreadstorage.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qthreadpool.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qthreadpool_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qthreadpool.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qsemaphore.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qsemaphore.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qtsan_impl.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qwaitcondition.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qwaitcondition_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qyieldcpu.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/time/qcalendar.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/time/qcalendar.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/time/qcalendarbackend_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/time/qcalendarmath_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/time/qdatetime.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/time/qdatetime.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/time/qdatetime_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/time/qgregoriancalendar.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/time/qgregoriancalendar_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/time/qjuliancalendar.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/time/qjuliancalendar_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/time/qlocaltime.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/time/qlocaltime_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/time/qmilankoviccalendar.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/time/qmilankoviccalendar_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/time/qromancalendar.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/time/qromancalendar_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/time/qtimezone.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/time/qtimezone.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qalgorithms.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qarraydata.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qarraydata.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qarraydataops.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qarraydatapointer.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qatomicscopedvaluerollback.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qbitarray.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qbitarray.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qcache.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qcontainerfwd.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qcontainertools_impl.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qcontiguouscache.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qcontiguouscache.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qcryptographichash.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qcryptographichash.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qduplicatetracker_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qflatmap_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qfreelist.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qfreelist_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qfunctionaltools_impl.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qfunctionaltools_impl.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qhashfunctions.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qiterator.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qline.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qline.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qlist.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qmakearray_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qmap.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qmargins.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qmargins.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qmessageauthenticationcode.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qminimalflatset_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qoffsetstringarray_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qpair.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qpoint.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qpoint.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qquasivirtual_impl.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qqueue.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qrect.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qrect.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qrefcount.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qrefcount.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qringbuffer.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qringbuffer_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qscopedpointer.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qscopedvaluerollback.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qscopeguard.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qset.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qshareddata.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qshareddata.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qshareddata_impl.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qsharedpointer.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qsharedpointer.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qsharedpointer_impl.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qsize.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qsize.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qsmallbytearray_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qspan.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qstack.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qtaggedpointer.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qtools_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qtyperevision.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qtyperevision.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/quniquehandle_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/quniquehandle_types.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/quniquehandle_types_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qvarlengtharray.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qvector.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qversionnumber.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qversionnumber.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/qtcore-config.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/qtcore-config_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qstringconverter_base_deprecated.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/ddf_Core.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qconfig.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qconfig_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qversiontagging.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qsimd_x86_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qxmlstream.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qxmlstream.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qxmlstream_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qxmlstreamgrammar.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qxmlstreamgrammar_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qxmlstreamparser_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/animation/qabstractanimation.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/animation/qabstractanimation.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/animation/qabstractanimation_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/animation/qanimationgroup.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/animation/qanimationgroup.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/animation/qanimationgroup_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/animation/qparallelanimationgroup.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/animation/qparallelanimationgroup.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/animation/qparallelanimationgroup_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/animation/qpauseanimation.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/animation/qpauseanimation.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/animation/qpropertyanimation.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/animation/qpropertyanimation.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/animation/qpropertyanimation_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/animation/qsequentialanimationgroup.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/animation/qsequentialanimationgroup.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/animation/qsequentialanimationgroup_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/animation/qvariantanimation.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/animation/qvariantanimation.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/animation/qvariantanimation_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qiooperation.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qiooperation_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qiooperation_p_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qrandomaccessasyncfile.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qrandomaccessasyncfile_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qrandomaccessasyncfile_p_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qrandomaccessasyncfile_qioring.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qrandomaccessasyncfile_threadpool.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qfutex_win_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qwaitcondition_win.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qoperatingsystemversion_win.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qoperatingsystemversion_win_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qt_windows.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qfilesystemengine_win.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qfsfileengine_win.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qlockfile_win.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qstandardpaths_win.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qstorageinfo_win.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qwindowspipereader.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qwindowspipereader_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qwindowspipewriter.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qwindowspipewriter_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qntdll_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qcoreapplication_win.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qeventdispatcher_win.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qeventdispatcher_win_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qfunctions_win.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qfunctions_win_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qfunctions_winrt_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/ipc/qsharedmemory_win.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/ipc/qsystemsemaphore_win.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qwineventnotifier.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qwineventnotifier.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qwineventnotifier_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qwinregistry.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qwinregistry_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/plugin/qsystemlibrary.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/plugin/qsystemlibrary_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qlocale_win.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qthread_win.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/platform/windows/qcomobject_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/platform/windows/qcomptr_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/platform/windows/qbstr_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/platform/windows/qcomvariant_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/platform/windows/quniquehandle_types_windows.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/platform/windows/quniquehandle_types_windows_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qioring.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qioring_win.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qioring_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qatomic.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qfutex_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qlatch.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qlatch_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qmutex.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qmutex_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qreadwritelock.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qreadwritelock_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qthreadstorage.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qthreadstorage_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qexception.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qexception.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qfuture.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qfuture_impl.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qfutureinterface.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qfutureinterface.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qfutureinterface_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qfuturesynchronizer.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qfuturewatcher.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qfuturewatcher.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qfuturewatcher_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qpromise.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qresultstore.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/thread/qresultstore.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qcommandlineoption.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qcommandlineoption.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qcommandlineparser.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qcommandlineparser.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/double-conversion/double-conversion/bignum.cc
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/double-conversion/double-conversion/bignum-dtoa.cc
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/double-conversion/double-conversion/bignum-dtoa.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/double-conversion/double-conversion/bignum.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/double-conversion/double-conversion/cached-powers.cc
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/double-conversion/double-conversion/cached-powers.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/double-conversion/double-conversion/diy-fp.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/double-conversion/double-conversion/double-conversion.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/double-conversion/double-conversion/double-to-string.cc
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/double-conversion/double-conversion/double-to-string.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/double-conversion/double-conversion/fast-dtoa.cc
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/double-conversion/double-conversion/fast-dtoa.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/double-conversion/double-conversion/fixed-dtoa.cc
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/double-conversion/double-conversion/fixed-dtoa.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/double-conversion/double-conversion/ieee.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/double-conversion/double-conversion/string-to-double.cc
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/double-conversion/double-conversion/string-to-double.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/double-conversion/double-conversion/strtod.cc
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/double-conversion/double-conversion/strtod.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/double-conversion/double-conversion/utils.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qeasingcurve.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qeasingcurve.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qtimeline.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/tools/qtimeline.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qcollator_win.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qregularexpression.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/text/qregularexpression.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/time/qhijricalendar.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/time/qhijricalendar_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/time/qislamiccivilcalendar.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/time/qislamiccivilcalendar_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/time/qjalalicalendar.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/time/qjalalicalendar_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/time/qtimezoneprivate.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/time/qtimezoneprivate_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/time/qtimezoneprivate_win.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/time/qtimezonelocale.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/time/qtimezonelocale_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/time/qdatetimeparser.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/time/qdatetimeparser_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qfilesystemwatcher.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qfilesystemwatcher.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qfilesystemwatcher_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qfilesystemwatcher_polling.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qfilesystemwatcher_polling_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qfilesystemwatcher_win.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qfilesystemwatcher_win_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qprocess.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qprocess.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qprocess_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qprocess_win.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qsettings.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qsettings.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qsettings_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qsettings_win.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/io/qfilesystemiterator_win.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/platform/windows/qfactorycacheregistration_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/platform/windows/qfactorycacheregistration.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/platform/windows/qt_winrtbase_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/itemmodels/qabstractitemmodel.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/itemmodels/qabstractitemmodel.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/itemmodels/qabstractitemmodel_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/itemmodels/qitemselectionmodel.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/itemmodels/qitemselectionmodel.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/itemmodels/qitemselectionmodel_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/itemmodels/qrangemodel.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/itemmodels/qrangemodel_impl.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/itemmodels/qrangemodel.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/itemmodels/qrangemodeladapter.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/itemmodels/qrangemodeladapter_impl.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/itemmodels/qabstractproxymodel.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/itemmodels/qabstractproxymodel.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/itemmodels/qabstractproxymodel_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/itemmodels/qconcatenatetablesproxymodel.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/itemmodels/qconcatenatetablesproxymodel.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/itemmodels/qidentityproxymodel.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/itemmodels/qidentityproxymodel.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/itemmodels/qidentityproxymodel_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/itemmodels/qsortfilterproxymodel.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/itemmodels/qsortfilterproxymodel.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/itemmodels/qtransposeproxymodel.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/itemmodels/qtransposeproxymodel.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/itemmodels/qtransposeproxymodel_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/itemmodels/qstringlistmodel.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/itemmodels/qstringlistmodel.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/plugin/qlibrary.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/plugin/qlibrary.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/plugin/qlibrary_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/plugin/qcoffpeparser.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/plugin/qcoffpeparser_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/plugin/qlibrary_win.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qcborstreamreader.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qcborstreamreader.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qcborstreamwriter.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/serialization/qcborstreamwriter.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/mimetypes/qmimedatabase.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/mimetypes/qmimedatabase.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/mimetypes/qmimedatabase_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/mimetypes/qmimeglobpattern.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/mimetypes/qmimeglobpattern_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/mimetypes/qmimemagicrule.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/mimetypes/qmimemagicrule_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/mimetypes/qmimemagicrulematcher.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/mimetypes/qmimemagicrulematcher_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/mimetypes/qmimeprovider.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/mimetypes/qmimeprovider_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/mimetypes/qmimetype.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/mimetypes/qmimetype.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/mimetypes/qmimetype_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/mimetypes/qmimetypeparser.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/mimetypes/qmimetypeparser_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qpermissions.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qpermissions.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/kernel/qpermissions_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/.rcc/qmimeprovider_database.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/doc/qtcore.qdocconf
Relationship: SPDXRef-PackagedFile-qt-module-Core-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/corelib/global/qtdeprecationdefinitions.h
"
                )
        endif()
