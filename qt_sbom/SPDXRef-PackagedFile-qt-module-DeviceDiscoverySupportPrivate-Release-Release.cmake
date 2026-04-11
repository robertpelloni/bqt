
        if(NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6DeviceDiscoverySupport.lib"
                AND NOT QT_SBOM_BUILD_TIME AND NOT QT_SBOM_FAKE_CHECKSUM)
            if(NOT TRUE)
                message(FATAL_ERROR "Cannot find 'lib/Qt6DeviceDiscoverySupport.lib' to compute its checksum. "
                    "Expected to find it at '$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6DeviceDiscoverySupport.lib' ")
            endif()
        else()
            if(NOT QT_SBOM_BUILD_TIME)
                if(QT_SBOM_FAKE_CHECKSUM)
                    set(sha1 "158942a783ee1095eafacaffd93de73edeadbeef")
                else()
                    file(SHA1 "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6DeviceDiscoverySupport.lib" sha1)
                endif()

                set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-module-DeviceDiscoverySupportPrivate" true)

                list(APPEND QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES
                    "SPDXRef-Package-qtbase-qt-module-DeviceDiscoverySupportPrivate")
                list(APPEND
                    "QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES_SPDXRef-Package-qtbase-qt-module-DeviceDiscoverySupportPrivate"
                    "${sha1}")
            endif()
            file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
FileName: ./lib/Qt6DeviceDiscoverySupport.lib
SPDXID: SPDXRef-PackagedFile-qt-module-DeviceDiscoverySupportPrivate-Release
FileType: BINARY
FileChecksum: SHA1: ${sha1}
LicenseConcluded: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
FileCopyrightText: <text>Copyright (C) The Qt Company Ltd. and other contributors.</text>
LicenseInfoInFile: NOASSERTION
Relationship: SPDXRef-Package-qtbase-qt-module-DeviceDiscoverySupportPrivate CONTAINS SPDXRef-PackagedFile-qt-module-DeviceDiscoverySupportPrivate-Release
Relationship: SPDXRef-PackagedFile-qt-module-DeviceDiscoverySupportPrivate-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/platformsupport/devicediscovery/qdevicediscovery_p.h
Relationship: SPDXRef-PackagedFile-qt-module-DeviceDiscoverySupportPrivate-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/platformsupport/devicediscovery/ddf_DeviceDiscoverySupportPrivate.h
Relationship: SPDXRef-PackagedFile-qt-module-DeviceDiscoverySupportPrivate-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/platformsupport/devicediscovery/qdevicediscovery_dummy.cpp
Relationship: SPDXRef-PackagedFile-qt-module-DeviceDiscoverySupportPrivate-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/platformsupport/devicediscovery/qdevicediscovery_dummy_p.h
"
                )
        endif()
