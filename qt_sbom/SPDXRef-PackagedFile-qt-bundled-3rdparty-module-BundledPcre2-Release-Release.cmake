
        if(NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6BundledPcre2.lib"
                AND NOT QT_SBOM_BUILD_TIME AND NOT QT_SBOM_FAKE_CHECKSUM)
            if(NOT TRUE)
                message(FATAL_ERROR "Cannot find 'lib/Qt6BundledPcre2.lib' to compute its checksum. "
                    "Expected to find it at '$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6BundledPcre2.lib' ")
            endif()
        else()
            if(NOT QT_SBOM_BUILD_TIME)
                if(QT_SBOM_FAKE_CHECKSUM)
                    set(sha1 "158942a783ee1095eafacaffd93de73edeadbeef")
                else()
                    file(SHA1 "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6BundledPcre2.lib" sha1)
                endif()

                set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledPcre2" true)

                list(APPEND QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES
                    "SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledPcre2")
                list(APPEND
                    "QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES_SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledPcre2"
                    "${sha1}")
            endif()
            file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
FileName: ./lib/Qt6BundledPcre2.lib
SPDXID: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledPcre2-Release
FileType: BINARY
FileChecksum: SHA1: ${sha1}
LicenseConcluded: LicenseRef-BSD-3-Clause-with-PCRE2-Binary-Like-Packages-Exception
FileCopyrightText: <text>Copyright (c) 1997-2007 University of Cambridge
Copyright (c) 2007-2024 Philip Hazel
Copyright (c) 2010-2024 Zoltan Herczeg</text>
LicenseInfoInFile: NOASSERTION
Relationship: SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledPcre2 CONTAINS SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledPcre2-Release
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledPcre2-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/pcre2/src/config.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledPcre2-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/pcre2/src/pcre2.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledPcre2-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/pcre2/src/pcre2_auto_possess.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledPcre2-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/pcre2/src/pcre2_chartables.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledPcre2-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/pcre2/src/pcre2_chkdint.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledPcre2-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/pcre2/src/pcre2_compile.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledPcre2-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/pcre2/src/pcre2_compile_cgroup.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledPcre2-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/pcre2/src/pcre2_compile_class.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledPcre2-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/pcre2/src/pcre2_config.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledPcre2-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/pcre2/src/pcre2_context.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledPcre2-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/pcre2/src/pcre2_dfa_match.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledPcre2-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/pcre2/src/pcre2_error.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledPcre2-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/pcre2/src/pcre2_extuni.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledPcre2-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/pcre2/src/pcre2_find_bracket.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledPcre2-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/pcre2/src/pcre2_internal.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledPcre2-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/pcre2/src/pcre2_intmodedep.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledPcre2-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/pcre2/src/pcre2_jit_compile.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledPcre2-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/pcre2/src/pcre2_maketables.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledPcre2-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/pcre2/src/pcre2_match.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledPcre2-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/pcre2/src/pcre2_match_data.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledPcre2-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/pcre2/src/pcre2_newline.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledPcre2-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/pcre2/src/pcre2_ord2utf.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledPcre2-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/pcre2/src/pcre2_pattern_info.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledPcre2-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/pcre2/src/pcre2_script_run.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledPcre2-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/pcre2/src/pcre2_serialize.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledPcre2-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/pcre2/src/pcre2_string_utils.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledPcre2-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/pcre2/src/pcre2_study.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledPcre2-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/pcre2/src/pcre2_substitute.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledPcre2-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/pcre2/src/pcre2_substring.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledPcre2-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/pcre2/src/pcre2_tables.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledPcre2-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/pcre2/src/pcre2_ucd.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledPcre2-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/pcre2/src/pcre2_ucp.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledPcre2-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/pcre2/src/pcre2_valid_utf.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledPcre2-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/pcre2/src/pcre2_xclass.c
"
                )
        endif()
