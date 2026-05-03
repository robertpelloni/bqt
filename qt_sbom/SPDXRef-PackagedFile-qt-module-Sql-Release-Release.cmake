
        if(NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6Sql.lib"
                AND NOT QT_SBOM_BUILD_TIME AND NOT QT_SBOM_FAKE_CHECKSUM)
            if(NOT TRUE)
                message(FATAL_ERROR "Cannot find 'lib/Qt6Sql.lib' to compute its checksum. "
                    "Expected to find it at '$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6Sql.lib' ")
            endif()
        else()
            if(NOT QT_SBOM_BUILD_TIME)
                if(QT_SBOM_FAKE_CHECKSUM)
                    set(sha1 "158942a783ee1095eafacaffd93de73edeadbeef")
                else()
                    file(SHA1 "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6Sql.lib" sha1)
                endif()

                set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-module-Sql" true)

                list(APPEND QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES
                    "SPDXRef-Package-qtbase-qt-module-Sql")
                list(APPEND
                    "QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES_SPDXRef-Package-qtbase-qt-module-Sql"
                    "${sha1}")
            endif()
            file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
FileName: ./lib/Qt6Sql.lib
SPDXID: SPDXRef-PackagedFile-qt-module-Sql-Release
FileType: BINARY
FileChecksum: SHA1: ${sha1}
LicenseConcluded: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
FileCopyrightText: <text>Copyright (C) The Qt Company Ltd. and other contributors.</text>
LicenseInfoInFile: NOASSERTION
Relationship: SPDXRef-Package-qtbase-qt-module-Sql CONTAINS SPDXRef-PackagedFile-qt-module-Sql-Release
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/include/QtSql/qtsqlexports.h
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/compat/removed_api.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/kernel/qsqlcachedresult.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/kernel/qsqlcachedresult_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/kernel/qsqldatabase.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/kernel/qsqldatabase.h
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/kernel/qsqldriver.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/kernel/qsqldriver.h
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/kernel/qsqldriver_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/kernel/qsqldriverplugin.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/kernel/qsqldriverplugin.h
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/kernel/qsqlerror.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/kernel/qsqlerror.h
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/kernel/qsqlfield.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/kernel/qsqlfield.h
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/kernel/qsqlindex.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/kernel/qsqlindex.h
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/kernel/qsqlnulldriver_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/kernel/qsqlquery.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/kernel/qsqlquery.h
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/kernel/qsqlrecord.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/kernel/qsqlrecord.h
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/kernel/qsqlresult.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/kernel/qsqlresult.h
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/kernel/qsqlresult_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/kernel/qtsqlglobal.h
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/kernel/qtsqlglobal_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/qtsql-config.h
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/qtsql-config_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/ddf_Sql.h
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/models/qsqlquerymodel.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/models/qsqlquerymodel.h
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/models/qsqlquerymodel_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/models/qsqlrelationaldelegate.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/models/qsqlrelationaldelegate.h
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/models/qsqlrelationaltablemodel.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/models/qsqlrelationaltablemodel.h
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/models/qsqltablemodel.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/models/qsqltablemodel.h
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/models/qsqltablemodel_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Sql-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/sql/doc/qtsql.qdocconf
"
                )
        endif()
