
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-system-3rdparty-WrapAtomic")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-system-3rdparty-WrapAtomic" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: WrapAtomic
SPDXID: SPDXRef-Package-qtbase-system-3rdparty-WrapAtomic
PackageDownloadLocation: NOASSERTION
PackageVersion: unknown
PackageSupplier: Person: Anonymous
PackageLicenseConcluded: NOASSERTION
PackageLicenseDeclared: NOASSERTION
PackageCopyrightText: NOASSERTION
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: WrapAtomic::WrapAtomic
</text>\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-system-3rdparty-WrapAtomic}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-system-3rdparty-WrapAtomic}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-system-3rdparty-WrapAtomic
"
        )
