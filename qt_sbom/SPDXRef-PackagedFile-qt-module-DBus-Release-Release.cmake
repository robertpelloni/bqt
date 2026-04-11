
        if(NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6DBus.lib"
                AND NOT QT_SBOM_BUILD_TIME AND NOT QT_SBOM_FAKE_CHECKSUM)
            if(NOT TRUE)
                message(FATAL_ERROR "Cannot find 'lib/Qt6DBus.lib' to compute its checksum. "
                    "Expected to find it at '$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6DBus.lib' ")
            endif()
        else()
            if(NOT QT_SBOM_BUILD_TIME)
                if(QT_SBOM_FAKE_CHECKSUM)
                    set(sha1 "158942a783ee1095eafacaffd93de73edeadbeef")
                else()
                    file(SHA1 "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6DBus.lib" sha1)
                endif()

                set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-module-DBus" true)

                list(APPEND QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES
                    "SPDXRef-Package-qtbase-qt-module-DBus")
                list(APPEND
                    "QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES_SPDXRef-Package-qtbase-qt-module-DBus"
                    "${sha1}")
            endif()
            file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
FileName: ./lib/Qt6DBus.lib
SPDXID: SPDXRef-PackagedFile-qt-module-DBus-Release
FileType: BINARY
FileChecksum: SHA1: ${sha1}
LicenseConcluded: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
FileCopyrightText: <text>Copyright (C) The Qt Company Ltd. and other contributors.</text>
LicenseInfoInFile: NOASSERTION
Relationship: SPDXRef-Package-qtbase-qt-module-DBus CONTAINS SPDXRef-PackagedFile-qt-module-DBus-Release
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/include/QtDBus/qtdbusexports.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/dbus_minimal_p.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbus_symbols.cpp
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbus_symbols_p.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusabstractadaptor.cpp
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusabstractadaptor.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusabstractadaptor_p.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusabstractinterface.cpp
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusabstractinterface.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusabstractinterface_p.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusargument.cpp
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusargument.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusargument_p.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusconnection.cpp
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusconnection.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusconnection_p.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusconnectioninterface.cpp
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusconnectioninterface.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusconnectionmanager.cpp
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusconnectionmanager_p.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbuscontext.cpp
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbuscontext.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbuscontext_p.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbuserror.cpp
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbuserror.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusextratypes.cpp
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusextratypes.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusintegrator.cpp
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusintegrator_p.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusinterface.cpp
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusinterface.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusinterface_p.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusinternalfilters.cpp
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusintrospection.cpp
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusintrospection_p.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusmarshaller.cpp
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusmessage.cpp
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusmessage.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusmessage_p.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusmetaobject.cpp
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusmetaobject_p.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusmetatype.cpp
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusmetatype.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusmetatype_p.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusmisc.cpp
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbuspendingcall.cpp
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbuspendingcall.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbuspendingcall_p.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbuspendingreply.cpp
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbuspendingreply.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusreply.cpp
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusreply.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusserver.cpp
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusserver.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusservicewatcher.cpp
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusservicewatcher.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusthreaddebug_p.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusunixfiledescriptor.cpp
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusunixfiledescriptor.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusutil.cpp
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusutil_p.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusvirtualobject.cpp
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusvirtualobject.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusxmlgenerator.cpp
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusxmlparser.cpp
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qdbusxmlparser_p.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qtdbusglobal.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/qtdbusglobal_p.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/ddf_DBus.h
Relationship: SPDXRef-PackagedFile-qt-module-DBus-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/dbus/doc/qtdbus.qdocconf
"
                )
        endif()
