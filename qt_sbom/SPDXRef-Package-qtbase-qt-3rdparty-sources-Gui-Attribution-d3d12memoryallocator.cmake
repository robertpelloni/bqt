
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-d3d12memoryallocator")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-d3d12memoryallocator" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: Gui_Attribution_d3d12memoryallocator
SPDXID: SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-d3d12memoryallocator
PackageDownloadLocation: https://github.com/GPUOpen-LibrariesAndSDKs/D3D12MemoryAllocator
PackageVersion: f128d39b7a95b4235bd228d231646278dc6c24b2
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: MIT
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:github/GPUOpen-LibrariesAndSDKs/D3D12MemoryAllocator@f128d39b7a95b4235bd228d231646278dc6c24b2
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=Gui_Attribution_d3d12memoryallocator#src/gui
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-Gui_Attribution_d3d12memoryallocator@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=Gui_Attribution_d3d12memoryallocator#src/gui
PackageCopyrightText: <text>Copyright (c) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: Gui_Attribution_d3d12memoryallocator
    Information extracted from:
     /src_dir/qtbase/src/3rdparty/D3D12MemoryAllocator/qt_attribution.json
    Entry index: 0
    Id: d3d12memoryallocator
    Name: D3D12 Memory Allocator
    Description: D3D12 Memory Allocator
    Qt usage: Memory management for the D3D12 backend of QRhi.
    License: MIT License
    License file: LICENSE.txt
</text>\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-d3d12memoryallocator}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-d3d12memoryallocator}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-d3d12memoryallocator
"
        )
