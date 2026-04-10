# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## Network Module:
#####################################################################

bobui_internal_add_module(Network
    PLUGIN_TYPES networkaccess networkinformation tls
    SOURCES
        access/qabstractnetworkcache.cpp access/qabstractnetworkcache.h access/qabstractnetworkcache_p.h
        access/qbytedatabuffer_p.h
        access/qhsts.cpp access/qhsts_p.h
        access/qhstspolicy.cpp access/qhstspolicy.h
        access/qnetworkaccessauthenticationmanager.cpp access/qnetworkaccessauthenticationmanager_p.h
        access/qnetworkaccessbackend.cpp access/qnetworkaccessbackend_p.h
        access/qnetworkaccesscache.cpp access/qnetworkaccesscache_p.h
        access/qnetworkaccesscachebackend.cpp access/qnetworkaccesscachebackend_p.h
        access/qnetworkaccessfilebackend.cpp access/qnetworkaccessfilebackend_p.h
        access/qnetworkaccessmanager.cpp access/qnetworkaccessmanager.h access/qnetworkaccessmanager_p.h
        access/qnetworkcookie.cpp access/qnetworkcookie.h access/qnetworkcookie_p.h
        access/qnetworkcookiejar.cpp access/qnetworkcookiejar.h access/qnetworkcookiejar_p.h
        access/qnetworkfile.cpp access/qnetworkfile_p.h
        access/qhttpheaders.cpp access/qhttpheaders.h
        access/qhttpheaderparser.cpp access/qhttpheaderparser_p.h
        access/qhttpheadershelper.cpp access/qhttpheadershelper_p.h
        access/qnetworkreply.cpp access/qnetworkreply.h access/qnetworkreply_p.h
        access/qnetworkreplydataimpl.cpp access/qnetworkreplydataimpl_p.h
        access/qnetworkreplyfileimpl.cpp access/qnetworkreplyfileimpl_p.h
        access/qnetworkreplyimpl.cpp access/qnetworkreplyimpl_p.h
        access/qnetworkrequest.cpp access/qnetworkrequest.h access/qnetworkrequest_p.h
        compat/removed_api.cpp
        kernel/qauthenticator.cpp kernel/qauthenticator.h kernel/qauthenticator_p.h
        kernel/qhostaddress.cpp kernel/qhostaddress.h kernel/qhostaddress_p.h
        kernel/qhostinfo.cpp kernel/qhostinfo.h kernel/qhostinfo_p.h
        kernel/qnetworkdatagram.cpp kernel/qnetworkdatagram.h kernel/qnetworkdatagram_p.h
        kernel/qnetworkinformation.cpp kernel/qnetworkinformation_p.h kernel/qnetworkinformation.h
        kernel/qnetworkinterface.cpp kernel/qnetworkinterface.h kernel/qnetworkinterface_p.h
        kernel/qnetworkinterface_unix_p.h
        kernel/qnetworkproxy.cpp kernel/qnetworkproxy.h
        kernel/bobuicontenttypeparser_p.h
        kernel/bobuinetworkglobal.h kernel/bobuinetworkglobal_p.h
        socket/qabstractsocket.cpp socket/qabstractsocket.h socket/qabstractsocket_p.h
        socket/qabstractsocketengine.cpp socket/qabstractsocketengine_p.h
        socket/qabstractsocketenginereceiver_p.h
        socket/qnativesocketengine.cpp socket/qnativesocketengine_p.h socket/qnativesocketengine_p_p.h
        socket/bobuicpserver.cpp socket/bobuicpserver.h socket/bobuicpserver_p.h
        socket/bobuicpsocket.cpp socket/bobuicpsocket.h socket/bobuicpsocket_p.h
        socket/qudpsocket.cpp socket/qudpsocket.h
        ssl/qpassworddigestor.cpp ssl/qpassworddigestor.h
        ssl/qssl.cpp ssl/qssl.h ssl/qssl_p.h
        ssl/qsslcertificate.cpp ssl/qsslcertificate.h ssl/qsslcertificate_p.h
        ssl/qsslcertificateextension.cpp ssl/qsslcertificateextension.h ssl/qsslcertificateextension_p.h
        ssl/qsslcipher.h
        ssl/qsslconfiguration.h
        ssl/qsslerror.h
        ssl/qsslkey.h
        ssl/qsslsocket.h
        ssl/bobuilsbackend.cpp ssl/bobuilsbackend_p.h
    DEFINES
        BOBUI_NO_CONTEXTLESS_CONNECT
        BOBUI_NO_FOREACH
        BOBUI_NO_SCOPED_POINTER
        BOBUI_NO_USING_NAMESPACE
        BOBUI_NO_CAST_FROM_ASCII
        BOBUI_NO_CAST_TO_ASCII
        BOBUI_NO_CAST_FROM_BYTEARRAY
        BOBUI_NO_URL_CAST_FROM_STRING
        BOBUI_USE_NODISCARD_FILE_OPEN
    INCLUDE_DIRECTORIES
        kernel
    LIBRARIES
        BobUI::CorePrivate
    PUBLIC_LIBRARIES
        BobUI::Core
    PRIVATE_MODULE_INTERFACE
        BobUI::CorePrivate
    NO_PCH_SOURCES
        compat/removed_api.cpp
    PRECOMPILED_HEADER
        "../corelib/global/bobui_pch.h"
)

## Scopes:
#####################################################################

bobui_internal_extend_target(Network CONDITION BOBUI_FEATURE_private_tests
    SOURCES
        access/qnetworkaccessdebugpipebackend.cpp access/qnetworkaccessdebugpipebackend_p.h
)

bobui_internal_extend_target(Network CONDITION MSVC AND (TEST_architecture_arch STREQUAL "i386")
    LINK_OPTIONS
        "/BASE:0x64000000"
)

bobui_internal_extend_target(Network CONDITION BOBUI_FEATURE_networkdiskcache
    SOURCES
        access/qnetworkdiskcache.cpp access/qnetworkdiskcache.h access/qnetworkdiskcache_p.h
)

bobui_internal_extend_target(Network CONDITION BOBUI_FEATURE_settings
    SOURCES
        access/qhstsstore.cpp access/qhstsstore_p.h
)

bobui_internal_extend_target(Network CONDITION APPLE
    LIBRARIES
        ${FWCoreFoundation}
        ${FWSecurity}
)

bobui_internal_extend_target(Network CONDITION WASM
    SOURCES
        access/qformdatabuilder.cpp access/qformdatabuilder.h
        access/qhttpmultipart.cpp access/qhttpmultipart.h access/qhttpmultipart_p.h
        access/qhttpnetworkheader.cpp access/qhttpnetworkheader_p.h
        access/qnetworkreplywasmimpl.cpp access/qnetworkreplywasmimpl_p.h
)

bobui_internal_extend_target(Network CONDITION BOBUI_FEATURE_http
    SOURCES
        access/http2/bitstreams.cpp access/http2/bitstreams_p.h
        access/http2/hpack.cpp access/http2/hpack_p.h
        access/http2/hpacktable.cpp access/http2/hpacktable_p.h
        access/http2/http2frames.cpp access/http2/http2frames_p.h
        access/http2/http2protocol.cpp access/http2/http2protocol_p.h
        access/http2/http2streams.cpp access/http2/http2streams_p.h
        access/http2/huffman.cpp access/http2/huffman_p.h
        access/qabstractprotocolhandler.cpp access/qabstractprotocolhandler_p.h
        access/qdecompresshelper.cpp access/qdecompresshelper_p.h
        access/qformdatabuilder.cpp access/qformdatabuilder.h
        access/qhttp1configuration.cpp access/qhttp1configuration.h
        access/qhttp2configuration.cpp access/qhttp2configuration.h
        access/qhttp2connection.cpp access/qhttp2connection_p.h
        access/qhttp2protocolhandler.cpp access/qhttp2protocolhandler_p.h
        access/qhttpmultipart.cpp access/qhttpmultipart.h access/qhttpmultipart_p.h
        access/qhttpnetworkconnection.cpp access/qhttpnetworkconnection_p.h
        access/qhttpnetworkconnectionchannel.cpp access/qhttpnetworkconnectionchannel_p.h
        access/qhttpnetworkheader.cpp access/qhttpnetworkheader_p.h
        access/qhttpnetworkreply.cpp access/qhttpnetworkreply_p.h
        access/qhttpnetworkrequest.cpp access/qhttpnetworkrequest_p.h
        access/qhttpprotocolhandler.cpp access/qhttpprotocolhandler_p.h
        access/qhttpthreaddelegate.cpp access/qhttpthreaddelegate_p.h
        access/qnetworkreplyhttpimpl.cpp access/qnetworkreplyhttpimpl_p.h
        access/qnetworkrequestfactory.cpp access/qnetworkrequestfactory_p.h
        access/qnetworkrequestfactory.h
        access/qrestaccessmanager.cpp access/qrestaccessmanager.h access/qrestaccessmanager_p.h
        access/qrestreply.cpp access/qrestreply.h access/qrestreply_p.h
        access/qsocketabstraction_p.h
        access/bobuicpkeepaliveconfiguration_p.h
        socket/qhttpsocketengine.cpp socket/qhttpsocketengine_p.h
)

bobui_internal_extend_target(Network CONDITION BOBUI_FEATURE_brotli AND BOBUI_FEATURE_http
    LIBRARIES
        WrapBrotli::WrapBrotliDec
)

bobui_internal_extend_target(Network CONDITION BOBUI_FEATURE_http AND BOBUI_FEATURE_zstd
    LIBRARIES
        WrapZSTD::WrapZSTD
)

bobui_internal_extend_target(Network CONDITION BOBUI_FEATURE_system_zlib
    LIBRARIES
        WrapZLIB::WrapZLIB
)

bobui_internal_extend_target(Network CONDITION NOT BOBUI_FEATURE_system_zlib
    INCLUDE_DIRECTORIES
        ../3rdparty/zlib/src
    ATTRIBUTION_FILE_DIR_PATHS
        ../3rdparty/zlib
)

bobui_internal_extend_target(Network CONDITION NOT BOBUI_FEATURE_system_zlib
    LIBRARIES
        BobUI::Core
)

bobui_internal_extend_target(Network CONDITION BOBUI_FEATURE_topleveldomain
    SOURCES
        kernel/bobuildurl.cpp kernel/bobuildurl_p.h
        ../3rdparty/libpsl/src/lookup_string_in_fixed_set.c
    INCLUDE_DIRECTORIES
        ../3rdparty/libpsl
    ATTRIBUTION_FILE_DIR_PATHS
        ../3rdparty/libpsl
)

bobui_internal_extend_target(Network CONDITION BOBUI_FEATURE_dnslookup
    SOURCES
        kernel/qdnslookup.cpp kernel/qdnslookup.h kernel/qdnslookup_p.h
)

bobui_internal_extend_target(Network CONDITION UNIX
    SOURCES
        kernel/qhostinfo_unix.cpp
        socket/qnativesocketengine_unix.cpp
        socket/qnet_unix_p.h
)

bobui_internal_extend_target(Network CONDITION BOBUI_FEATURE_linux_netlink AND UNIX
    SOURCES
        kernel/qnetworkinterface_linux.cpp
)

bobui_internal_extend_target(Network CONDITION UNIX AND NOT BOBUI_FEATURE_linux_netlink
    SOURCES
        kernel/qnetworkinterface_unix.cpp
)

bobui_internal_extend_target(Network CONDITION WIN32
    SOURCES
        kernel/qhostinfo_win.cpp
        kernel/qnetworkinterface_win.cpp
        kernel/qnetworkproxy_win.cpp
        socket/qnativesocketengine_win.cpp
    LIBRARIES
        advapi32
        dnsapi
        iphlpapi
        secur32
        winhttp
    DEFINES
        NOMINMAX
)

bobui_internal_extend_target(Network CONDITION APPLE AND NOT UIKIT
    LIBRARIES
        ${FWCoreServices}
        ${FWSystemConfiguration}
)

bobui_internal_extend_target(Network CONDITION APPLE
    LIBRARIES
        ${FWCFNetwork}
)

bobui_internal_extend_target(Network CONDITION BOBUI_FEATURE_dnslookup AND BOBUI_FEATURE_libresolv
    SOURCES
        kernel/qdnslookup_unix.cpp
    LIBRARIES
        WrapResolv::WrapResolv
)

bobui_internal_extend_target(Network CONDITION BOBUI_FEATURE_dnslookup AND WIN32
    SOURCES
        kernel/qdnslookup_win.cpp
)

bobui_internal_extend_target(Network CONDITION BOBUI_FEATURE_dnslookup AND NOT BOBUI_FEATURE_libresolv AND NOT WIN32
    SOURCES
        kernel/qdnslookup_dummy.cpp
)

bobui_internal_extend_target(Network CONDITION APPLE
    LIBRARIES
        ${FWNetwork}
)

bobui_internal_extend_target(Network CONDITION BOBUI_FEATURE_gssapi
    LIBRARIES
        GSSAPI::GSSAPI
)

bobui_internal_extend_target(Network CONDITION UIKIT
    SOURCES
        kernel/qnetworkinterface_uikit_p.h
)

bobui_internal_extend_target(Network CONDITION APPLE AND NOT VISIONOS
    SOURCES
        kernel/qnetworkproxy_darwin.cpp
)

bobui_internal_extend_target(Network CONDITION BOBUI_FEATURE_libproxy AND UNIX AND NOT MACOS
    SOURCES
        kernel/qnetworkproxy_libproxy.cpp
    LIBRARIES
        ${CMAKE_DL_LIBS}
        PkgConfig::Libproxy
)

bobui_internal_extend_target(Network CONDITION ANDROID
    SOURCES
        kernel/qnetworkproxy_android.cpp
)

bobui_internal_extend_target(Network CONDITION UNIX AND NOT ANDROID AND NOT (APPLE AND NOT VISIONOS) AND NOT BOBUI_FEATURE_libproxy AND (UNIX OR WINRT)
    SOURCES
        kernel/qnetworkproxy_generic.cpp
)

if(ANDROID AND (ANDROID))
    set_property(TARGET Network APPEND PROPERTY BOBUI_ANDROID_BUNDLED_JAR_DEPENDENCIES
        jar/BobUI${BobUIBase_VERSION_MAJOR}AndroidNetwork.jar
    )
endif()

bobui_internal_extend_target(Network CONDITION BOBUI_FEATURE_socks5
    SOURCES
        socket/qsocks5socketengine.cpp socket/qsocks5socketengine_p.h
)

bobui_internal_extend_target(Network CONDITION BOBUI_FEATURE_sctp
    SOURCES
        socket/qsctpserver.cpp socket/qsctpserver.h socket/qsctpserver_p.h
        socket/qsctpsocket.cpp socket/qsctpsocket.h socket/qsctpsocket_p.h
)

bobui_internal_extend_target(Network CONDITION MSVC
    MOC_OPTIONS
        "-D_WINSOCK_DEPRECATED_NO_WARNINGS"
)

bobui_internal_extend_target(Network CONDITION BOBUI_FEATURE_localserver
    SOURCES
        socket/qlocalserver.cpp socket/qlocalserver.h socket/qlocalserver_p.h
        socket/qlocalsocket.cpp socket/qlocalsocket.h socket/qlocalsocket_p.h
)

bobui_internal_extend_target(Network CONDITION INTEGRITY AND BOBUI_FEATURE_localserver
    SOURCES
        socket/qlocalserver_tcp.cpp
        socket/qlocalsocket_tcp.cpp
    DEFINES
        BOBUI_LOCALSOCKET_TCP
)

bobui_internal_extend_target(Network CONDITION BOBUI_FEATURE_localserver AND UNIX AND NOT INTEGRITY
    SOURCES
        socket/qlocalserver_unix.cpp
        socket/qlocalsocket_unix.cpp
)

bobui_internal_extend_target(Network CONDITION BOBUI_FEATURE_localserver AND WIN32
    SOURCES
        socket/qlocalserver_win.cpp
        socket/qlocalsocket_win.cpp
)

bobui_internal_extend_target(Network CONDITION BOBUI_FEATURE_openssl_linked AND BOBUI_FEATURE_opensslv30
    LIBRARIES
        WrapOpenSSL::WrapOpenSSL
)

bobui_internal_extend_target(Network CONDITION BOBUI_FEATURE_system_proxies
    DEFINES
        BOBUI_USE_SYSTEM_PROXIES
)

bobui_internal_extend_target(Network CONDITION BOBUI_FEATURE_ssl
    SOURCES
        ssl/qocspresponse.cpp ssl/qocspresponse.h ssl/qocspresponse_p.h
        ssl/qsslcipher.cpp ssl/qsslcipher_p.h
        ssl/qsslconfiguration.cpp ssl/qsslconfiguration_p.h
        ssl/qssldiffiehellmanparameters.cpp ssl/qssldiffiehellmanparameters.h ssl/qssldiffiehellmanparameters_p.h
        ssl/qsslellipticcurve.cpp ssl/qsslellipticcurve.h
        ssl/qsslerror.cpp
        ssl/qsslkey_p.cpp ssl/qsslkey_p.h
        ssl/qsslpresharedkeyauthenticator.cpp ssl/qsslpresharedkeyauthenticator.h ssl/qsslpresharedkeyauthenticator_p.h
        ssl/qsslsocket.cpp ssl/qsslsocket_p.h
        ssl/qsslserver.cpp ssl/qsslserver.h ssl/qsslserver_p.h
)

bobui_internal_extend_target(Network CONDITION BOBUI_FEATURE_dtls AND BOBUI_FEATURE_ssl
    SOURCES
        ssl/qdtls.cpp ssl/qdtls.h ssl/qdtls_p.h
)

bobui_internal_extend_target(Network CONDITION BOBUI_FEATURE_ocsp AND BOBUI_FEATURE_openssl AND BOBUI_FEATURE_ssl
    SOURCES
        ssl/qocsp_p.h
)

bobui_internal_add_docs(Network
    doc/bobuinetwork.qdocconf
    SKIP_JAVADOC
)

# See mkspecs/common/msvc-desktop.conf
bobui_internal_extend_target(Network CONDITION WIN32 PUBLIC_LIBRARIES ws2_32)

# See mkspecs/common/qcc-base-qnx.conf
bobui_internal_extend_target(Network CONDITION QNX PUBLIC_LIBRARIES socket)

bobui_internal_extend_target(Network CONDITION SOLARIS PUBLIC_LIBRARIES socket nsl)

bobui_internal_extend_target(Network CONDITION WIN32
    NO_UNITY_BUILD_SOURCES
        kernel/qauthenticator.cpp
        kernel/qdnslookup_win.cpp
        kernel/qhostaddress.cpp
        kernel/qhostinfo.cpp
        kernel/qhostinfo_win.cpp
        kernel/qnetworkinterface_win.cpp
        kernel/qnetworkproxy_win.cpp
        socket/qabstractsocket.cpp
        socket/qabstractsocketengine.cpp
        socket/qlocalserver.cpp
        socket/qlocalserver_win.cpp
        socket/qlocalsocket_win.cpp
        socket/qnativesocketengine.cpp
        socket/qnativesocketengine_win.cpp
)

if(BOBUI_BUILD_DOC_SNIPPETS)
    add_subdirectory(doc/snippets/network)
endif()

bobui_internal_extend_target(Network
    # Workaround for BOBUIBUG-118229:
    # Function called by inline methods taking a pointer to a private class as a parameter
    EXTRA_ELF_LINKER_SCRIPT_EXPORTS
        # QNetworkDatagram::destroy(QNetworkDatagramPrivate *d)
        "_ZN*16QNetworkDatagram7destroyEP*23QNetworkDatagramPrivate*"
)
