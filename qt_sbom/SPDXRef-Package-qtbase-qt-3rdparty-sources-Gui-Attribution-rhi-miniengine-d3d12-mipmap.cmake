
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-rhi-miniengine-d3d12-mipmap")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-rhi-miniengine-d3d12-mipmap" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: Gui_Attribution_rhi-miniengine-d3d12-mipmap
SPDXID: SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-rhi-miniengine-d3d12-mipmap
PackageDownloadLocation: https://github.com/microsoft/DirectX-Graphics-Samples
PackageVersion: 0aa79bad78992da0b6a8279ddb9002c1753cb849
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: MIT
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:github/microsoft/DirectX-Graphics-Samples@0aa79bad78992da0b6a8279ddb9002c1753cb849
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=Gui_Attribution_rhi-miniengine-d3d12-mipmap#src/gui
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-Gui_Attribution_rhi-miniengine-d3d12-mipmap@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=Gui_Attribution_rhi-miniengine-d3d12-mipmap#src/gui
PackageCopyrightText: <text>Copyright (c) 2015 Microsoft</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: Gui_Attribution_rhi-miniengine-d3d12-mipmap
    Information extracted from:
     /src_dir/qtbase/src/gui/rhi/qt_attribution.json
    Entry index: 0
    Id: rhi-miniengine-d3d12-mipmap
    Name: Mipmap generator for D3D12
    Description: Compute shader for mipmap generation from MiniEngine in DirectX-Graphics-Samples
    Qt usage: Compute shader for mipmap generation with Direct 3D 12
    License: MIT License
    License file: LICENSE.MiniEngine.txt
</text>\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-rhi-miniengine-d3d12-mipmap}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-rhi-miniengine-d3d12-mipmap}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-rhi-miniengine-d3d12-mipmap
"
        )
