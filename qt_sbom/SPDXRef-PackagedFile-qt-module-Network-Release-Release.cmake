
        if(NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6Network.lib"
                AND NOT QT_SBOM_BUILD_TIME AND NOT QT_SBOM_FAKE_CHECKSUM)
            if(NOT TRUE)
                message(FATAL_ERROR "Cannot find 'lib/Qt6Network.lib' to compute its checksum. "
                    "Expected to find it at '$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6Network.lib' ")
            endif()
        else()
            if(NOT QT_SBOM_BUILD_TIME)
                if(QT_SBOM_FAKE_CHECKSUM)
                    set(sha1 "158942a783ee1095eafacaffd93de73edeadbeef")
                else()
                    file(SHA1 "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6Network.lib" sha1)
                endif()

                set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-module-Network" true)

                list(APPEND QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES
                    "SPDXRef-Package-qtbase-qt-module-Network")
                list(APPEND
                    "QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES_SPDXRef-Package-qtbase-qt-module-Network"
                    "${sha1}")
            endif()
            file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
FileName: ./lib/Qt6Network.lib
SPDXID: SPDXRef-PackagedFile-qt-module-Network-Release
FileType: BINARY
FileChecksum: SHA1: ${sha1}
LicenseConcluded: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
FileCopyrightText: <text>Copyright (C) The Qt Company Ltd. and other contributors.</text>
LicenseInfoInFile: NOASSERTION
Relationship: SPDXRef-Package-qtbase-qt-module-Network CONTAINS SPDXRef-PackagedFile-qt-module-Network-Release
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/include/QtNetwork/qtnetworkexports.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qabstractnetworkcache.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qabstractnetworkcache.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qabstractnetworkcache_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qbytedatabuffer_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhsts.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhsts_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhstspolicy.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhstspolicy.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkaccessauthenticationmanager.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkaccessauthenticationmanager_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkaccessbackend.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkaccessbackend_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkaccesscache.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkaccesscache_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkaccesscachebackend.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkaccesscachebackend_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkaccessfilebackend.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkaccessfilebackend_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkaccessmanager.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkaccessmanager.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkaccessmanager_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkcookie.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkcookie.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkcookie_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkcookiejar.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkcookiejar.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkcookiejar_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkfile.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkfile_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhttpheaders.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhttpheaders.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhttpheaderparser.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhttpheaderparser_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhttpheadershelper.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhttpheadershelper_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkreply.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkreply.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkreply_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkreplydataimpl.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkreplydataimpl_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkreplyfileimpl.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkreplyfileimpl_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkreplyimpl.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkreplyimpl_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkrequest.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkrequest.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkrequest_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/compat/removed_api.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/kernel/qauthenticator.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/kernel/qauthenticator.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/kernel/qauthenticator_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/kernel/qhostaddress.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/kernel/qhostaddress.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/kernel/qhostaddress_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/kernel/qhostinfo.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/kernel/qhostinfo.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/kernel/qhostinfo_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/kernel/qnetworkdatagram.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/kernel/qnetworkdatagram.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/kernel/qnetworkdatagram_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/kernel/qnetworkinformation.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/kernel/qnetworkinformation_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/kernel/qnetworkinformation.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/kernel/qnetworkinterface.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/kernel/qnetworkinterface.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/kernel/qnetworkinterface_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/kernel/qnetworkinterface_unix_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/kernel/qnetworkproxy.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/kernel/qnetworkproxy.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/kernel/qtcontenttypeparser_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/kernel/qtnetworkglobal.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/kernel/qtnetworkglobal_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/socket/qabstractsocket.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/socket/qabstractsocket.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/socket/qabstractsocket_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/socket/qabstractsocketengine.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/socket/qabstractsocketengine_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/socket/qabstractsocketenginereceiver_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/socket/qnativesocketengine.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/socket/qnativesocketengine_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/socket/qnativesocketengine_p_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/socket/qtcpserver.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/socket/qtcpserver.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/socket/qtcpserver_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/socket/qtcpsocket.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/socket/qtcpsocket.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/socket/qtcpsocket_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/socket/qudpsocket.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/socket/qudpsocket.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qpassworddigestor.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qpassworddigestor.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qssl.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qssl.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qssl_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qsslcertificate.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qsslcertificate.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qsslcertificate_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qsslcertificateextension.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qsslcertificateextension.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qsslcertificateextension_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qsslcipher.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qsslconfiguration.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qsslerror.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qsslkey.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qsslsocket.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qtlsbackend.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qtlsbackend_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/qtnetwork-config.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/qtnetwork-config_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ddf_Network.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkdiskcache.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkdiskcache.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkdiskcache_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhstsstore.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhstsstore_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/http2/bitstreams.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/http2/bitstreams_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/http2/hpack.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/http2/hpack_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/http2/hpacktable.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/http2/hpacktable_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/http2/http2frames.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/http2/http2frames_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/http2/http2protocol.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/http2/http2protocol_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/http2/http2streams.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/http2/http2streams_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/http2/huffman.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/http2/huffman_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qabstractprotocolhandler.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qabstractprotocolhandler_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qdecompresshelper.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qdecompresshelper_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qformdatabuilder.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qformdatabuilder.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhttp1configuration.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhttp1configuration.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhttp2configuration.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhttp2configuration.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhttp2connection.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhttp2connection_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhttp2protocolhandler.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhttp2protocolhandler_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhttpmultipart.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhttpmultipart.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhttpmultipart_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhttpnetworkconnection.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhttpnetworkconnection_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhttpnetworkconnectionchannel.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhttpnetworkconnectionchannel_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhttpnetworkheader.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhttpnetworkheader_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhttpnetworkreply.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhttpnetworkreply_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhttpnetworkrequest.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhttpnetworkrequest_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhttpprotocolhandler.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhttpprotocolhandler_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhttpthreaddelegate.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qhttpthreaddelegate_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkreplyhttpimpl.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkreplyhttpimpl_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkrequestfactory.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkrequestfactory_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qnetworkrequestfactory.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qrestaccessmanager.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qrestaccessmanager.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qrestaccessmanager_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qrestreply.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qrestreply.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qrestreply_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qsocketabstraction_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/access/qtcpkeepaliveconfiguration_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/socket/qhttpsocketengine.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/socket/qhttpsocketengine_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/kernel/qtldurl.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/kernel/qtldurl_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libpsl/src/lookup_string_in_fixed_set.c
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/kernel/qdnslookup.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/kernel/qdnslookup.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/kernel/qdnslookup_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/kernel/qhostinfo_win.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/kernel/qnetworkinterface_win.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/kernel/qnetworkproxy_win.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/socket/qnativesocketengine_win.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/kernel/qdnslookup_win.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/socket/qsocks5socketengine.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/socket/qsocks5socketengine_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/socket/qlocalserver.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/socket/qlocalserver.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/socket/qlocalserver_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/socket/qlocalsocket.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/socket/qlocalsocket.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/socket/qlocalsocket_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/socket/qlocalserver_win.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/socket/qlocalsocket_win.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qocspresponse.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qocspresponse.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qocspresponse_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qsslcipher.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qsslcipher_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qsslconfiguration.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qsslconfiguration_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qssldiffiehellmanparameters.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qssldiffiehellmanparameters.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qssldiffiehellmanparameters_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qsslellipticcurve.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qsslellipticcurve.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qsslerror.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qsslkey_p.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qsslkey_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qsslpresharedkeyauthenticator.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qsslpresharedkeyauthenticator.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qsslpresharedkeyauthenticator_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qsslsocket.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qsslsocket_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qsslserver.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qsslserver.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qsslserver_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qdtls.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qdtls.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qdtls_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/ssl/qocsp_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Network-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/network/doc/qtnetwork.qdocconf
"
                )
        endif()
