
        if(NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6BundledLibpng.lib"
                AND NOT QT_SBOM_BUILD_TIME AND NOT QT_SBOM_FAKE_CHECKSUM)
            if(NOT TRUE)
                message(FATAL_ERROR "Cannot find 'lib/Qt6BundledLibpng.lib' to compute its checksum. "
                    "Expected to find it at '$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6BundledLibpng.lib' ")
            endif()
        else()
            if(NOT QT_SBOM_BUILD_TIME)
                if(QT_SBOM_FAKE_CHECKSUM)
                    set(sha1 "158942a783ee1095eafacaffd93de73edeadbeef")
                else()
                    file(SHA1 "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6BundledLibpng.lib" sha1)
                endif()

                set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledLibpng" true)

                list(APPEND QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES
                    "SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledLibpng")
                list(APPEND
                    "QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES_SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledLibpng"
                    "${sha1}")
            endif()
            file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
FileName: ./lib/Qt6BundledLibpng.lib
SPDXID: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibpng-Release
FileType: BINARY
FileChecksum: SHA1: ${sha1}
LicenseConcluded: Libpng AND libpng-2.0
FileCopyrightText: <text>Copyright (c) 1995-2026 The PNG Reference Library Authors
Copyright (c) 2000-2026 Cosmin Truta
Copyright (c) 1998-2018 Glenn Randers-Pehrson
Copyright (c) 1996-1997 Andreas Dilger
Copyright (c) 1995-1996 Guy Eric Schalnat, Group 42, Inc.
Copyright (c) 2000-2017 Simon-Pierre Cadieux
Copyright (c) 2000-2017 Eric S. Raymond
Copyright (c) 2000-2017 Mans Rullgard
Copyright (c) 2000-2017 Gilles Vollant
Copyright (c) 2000-2017 James Yu
Copyright (c) 2000-2017 Mandar Sahastrabuddhe
Copyright (c) 1998-2000 Tom Lane
Copyright (c) 1998-2000 Willem van Schaik
Copyright (c) 1996-1997 John Bowler
Copyright (c) 1996-1997 Kevin Bracey
Copyright (c) 1996-1997 Sam Bushell
Copyright (c) 1996-1997 Magnus Holmgren
Copyright (c) 1996-1997 Greg Roelofs
Copyright (c) 1996-1997 Tom Tanner
Copyright (c) 1995-1996 Dave Martindale
Copyright (c) 1995-1996 Paul Schmidt
Copyright (c) 1995-1996 Tim Wegner</text>
LicenseInfoInFile: NOASSERTION
Relationship: SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledLibpng CONTAINS SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibpng-Release
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibpng-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libpng/png.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibpng-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libpng/pngerror.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibpng-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libpng/pngget.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibpng-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libpng/pngmem.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibpng-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libpng/pngpread.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibpng-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libpng/pngread.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibpng-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libpng/pngrio.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibpng-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libpng/pngrtran.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibpng-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libpng/pngrutil.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibpng-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libpng/pngset.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibpng-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libpng/pngtrans.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibpng-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libpng/pngwio.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibpng-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libpng/pngwrite.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibpng-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libpng/pngwtran.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibpng-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libpng/pngwutil.c
"
                )
        endif()
