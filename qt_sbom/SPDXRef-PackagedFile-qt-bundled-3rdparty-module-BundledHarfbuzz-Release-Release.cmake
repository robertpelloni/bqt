
        if(NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6BundledHarfbuzz.lib"
                AND NOT QT_SBOM_BUILD_TIME AND NOT QT_SBOM_FAKE_CHECKSUM)
            if(NOT TRUE)
                message(FATAL_ERROR "Cannot find 'lib/Qt6BundledHarfbuzz.lib' to compute its checksum. "
                    "Expected to find it at '$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6BundledHarfbuzz.lib' ")
            endif()
        else()
            if(NOT QT_SBOM_BUILD_TIME)
                if(QT_SBOM_FAKE_CHECKSUM)
                    set(sha1 "158942a783ee1095eafacaffd93de73edeadbeef")
                else()
                    file(SHA1 "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6BundledHarfbuzz.lib" sha1)
                endif()

                set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledHarfbuzz" true)

                list(APPEND QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES
                    "SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledHarfbuzz")
                list(APPEND
                    "QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES_SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledHarfbuzz"
                    "${sha1}")
            endif()
            file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
FileName: ./lib/Qt6BundledHarfbuzz.lib
SPDXID: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release
FileType: BINARY
FileChecksum: SHA1: ${sha1}
LicenseConcluded: MIT
FileCopyrightText: <text>Copyright © 2010-2022  Google, Inc.
Copyright © 2015-2020  Ebrahim Byagowi
Copyright © 2019,2020  Facebook, Inc.
Copyright © 2012,2015  Mozilla Foundation
Copyright © 2011  Codethink Limited
Copyright © 2008,2010  Nokia Corporation and/or its subsidiary(-ies)
Copyright © 2009  Keith Stribley
Copyright © 2011  Martin Hosken and SIL International
Copyright © 2007  Chris Wilson
Copyright © 2005,2006,2020,2021,2022,2023  Behdad Esfahbod
Copyright © 2004,2007,2008,2009,2010,2013,2021,2022,2023  Red Hat, Inc.
Copyright © 1998-2005  David Turner and Werner Lemberg
Copyright © 2016  Igalia S.L.
Copyright © 2022  Matthias Clasen
Copyright © 2018,2021  Khaled Hosny
Copyright © 2018,2019,2020  Adobe, Inc
Copyright © 2013-2015  Alexei Podtelezhnikov</text>
LicenseInfoInFile: NOASSERTION
Relationship: SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledHarfbuzz CONTAINS SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/hb-dummy.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-aat-layout.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-aat-map.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-algs.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-atomic.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-bit-vector.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-blob.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-blob.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-buffer.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-buffer.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-buffer.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-buffer-deserialize-json.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-buffer-deserialize-text-glyphs.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-buffer-deserialize-text-unicode.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-buffer-serialize.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-buffer-verify.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-cache.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-common.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-debug.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-decycler.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-deprecated.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-draw.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-draw.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-draw.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-face.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-face.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-face.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-face-builder.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-fallback-shape.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-font.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-font.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-font.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ft-colr.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-geometry.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-limits.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-map.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-mutex.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-number.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-object.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-open-file.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-open-type.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-outline.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-outline.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-paint.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-paint.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-paint.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-paint-bounded.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-paint-bounded.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-paint-extents.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-paint-extents.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-priority-queue.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-repacker.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-script-list.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-set.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-set.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-set.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-set-digest.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-shape.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-shape.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-shape-plan.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-shape-plan.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-shape-plan.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-shaper.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-shaper.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-shaper-impl.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-shaper-list.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-string-array.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-style.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-style.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-subset.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-subset-cff-common.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-subset-cff1.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-subset-cff2.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-subset-input.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-subset-instancer-iup.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-subset-instancer-iup.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-subset-instancer-solver.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-subset-plan.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-subset-plan-layout.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-subset-plan-member-list.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-subset-serialize.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-subset-serialize.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-subset-table-cff.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-subset-table-color.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-subset-table-layout.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-subset-table-other.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-subset-table-var.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-subset-table.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-subset-plan-var.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-unicode.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-unicode.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-unicode.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-utf.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-version.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-cff1-std-str.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-cff1-table.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-cff2-table.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-cmap-table.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-color.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-face.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-font.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-font.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-glyf-table.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-head-table.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-hhea-table.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-hmtx-table.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-kern-table.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-layout.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-layout.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-layout.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-layout-gdef-table.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-layout-gpos-table.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-layout-gsub-table.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-layout-jstf-table.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-map.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-map.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-math.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-math.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-math-table.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-maxp-table.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-meta.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-metrics.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-name.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-name-table.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-os2-table.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-post-macroman.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-post-table.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-post-table-v2subset.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-shape.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-shape.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-shape.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-shape-fallback.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-shape-fallback.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-shape-normalize.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-shape-normalize.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-shaper-arabic-fallback.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-shaper-arabic-joining-list.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-shaper-arabic-pua.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-shaper-arabic-table.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-shaper-arabic.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-shaper-arabic.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-shaper-default.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-shaper-hangul.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-shaper-hebrew.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-shaper-indic-machine.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-shaper-indic-table.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-shaper-indic.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-shaper-indic.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-shaper-khmer-machine.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-shaper-khmer.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-shaper-myanmar-machine.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-shaper-myanmar.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-shaper-syllabic.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-shaper-syllabic.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-shaper-thai.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-shaper-use-machine.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-shaper-use-table.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-shaper-use.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-shaper-vowel-constraints.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-shaper-vowel-constraints.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-shaper.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-subset-accelerator.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-tag.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-var.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-var.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-var-common.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-var-avar-table.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-var-fvar-table.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-var-hvar-table.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-var-mvar-table.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/hb-ot-var-varc-table.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/OT/Color/CBDT/CBDT.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/OT/Color/COLR/COLR.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/OT/Color/COLR/colrv1-closure.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/OT/Color/CPAL/CPAL.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/OT/Color/sbix/sbix.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/OT/Color/svg/svg.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/OT/Layout/GDEF/GDEF.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/OT/name/name.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/OT/Var/VARC/coord-setter.hh
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/OT/Var/VARC/VARC.cc
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledHarfbuzz-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/harfbuzz-ng/src/OT/Var/VARC/VARC.hh
"
                )
        endif()
