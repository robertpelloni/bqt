
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-system-3rdparty-ODBC")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-system-3rdparty-ODBC" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: ODBC
SPDXID: SPDXRef-Package-qtbase-system-3rdparty-ODBC
PackageDownloadLocation: NOASSERTION
PackageVersion: unknown
PackageSupplier: Person: Anonymous
PackageLicenseConcluded: NOASSERTION
PackageLicenseDeclared: NOASSERTION
PackageCopyrightText: NOASSERTION
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: ODBC::ODBC
</text>\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-system-3rdparty-ODBC}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-system-3rdparty-ODBC}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-system-3rdparty-ODBC
"
        )
