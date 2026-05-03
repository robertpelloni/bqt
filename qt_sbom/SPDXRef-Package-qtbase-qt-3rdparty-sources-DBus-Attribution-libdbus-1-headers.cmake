
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-3rdparty-sources-DBus-Attribution-libdbus-1-headers")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-DBus-Attribution-libdbus-1-headers" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: DBus_Attribution_libdbus-1-headers
SPDXID: SPDXRef-Package-qtbase-qt-3rdparty-sources-DBus-Attribution-libdbus-1-headers
PackageDownloadLocation: https://www.freedesktop.org/wiki/Software/dbus/
PackageVersion: 1.13.12
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: AFL-2.1 OR GPL-2.0-or-later
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:github/d-bus/dbus@dbus-1.13.12
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=DBus_Attribution_libdbus-1-headers#src/dbus
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-DBus_Attribution_libdbus-1-headers@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=DBus_Attribution_libdbus-1-headers#src/dbus
PackageCopyrightText: <text>Copyright (C) 2002, 2003 CodeFactory AB
Copyright (C) 2004, 2005 Red Hat, Inc.</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: DBus_Attribution_libdbus-1-headers
    Information extracted from:
     /src_dir/qtbase/src/dbus/qt_attribution.json
    Entry index: 0
    Id: libdbus-1-headers
    Name: libdbus-1 headers
    Description: D-Bus is a message bus system, a simple way for applications to talk to one another.
    Qt usage: Qt D-Bus uses constants and typedefs from libdbus-1 headers.
    License: Academic Free License v2.1, or GNU General Public License v2.0 or later
    License file: LICENSE.LIBDBUS-1.txt
</text>
ExternalRef: SECURITY cpe23Type cpe:2.3:a:freedesktop:dbus:1.13.12:*:*:*:*:*:*:*\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-3rdparty-sources-DBus-Attribution-libdbus-1-headers}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-DBus-Attribution-libdbus-1-headers}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-3rdparty-sources-DBus-Attribution-libdbus-1-headers
"
        )
