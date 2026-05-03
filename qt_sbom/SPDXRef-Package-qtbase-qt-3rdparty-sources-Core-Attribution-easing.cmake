
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-easing")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-easing" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: Core_Attribution_easing
SPDXID: SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-easing
PackageDownloadLocation: http://robertpenner.com/easing/
PackageVersion: unknown
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: BSD-3-Clause
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=Core_Attribution_easing#src/corelib
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-Core_Attribution_easing@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=Core_Attribution_easing#src/corelib
PackageCopyrightText: <text>Copyright (c) 2001 Robert Penner</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: Core_Attribution_easing
    Information extracted from:
     /src_dir/qtbase/src/3rdparty/easing/qt_attribution.json
    Entry index: 0
    Id: easing
    Name: Easing Equations by Robert Penner
    Qt usage: Used in Qt Core (QEasingCurve).
    License: BSD 3-clause \"New\" or \"Revised\" License
    License file: LICENSE
</text>\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-easing}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-easing}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-easing
"
        )
