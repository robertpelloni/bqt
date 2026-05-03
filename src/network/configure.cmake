# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#### Inputs



#### Libraries

bobui_feature_vcpkg_scope(network)
bobui_find_package(WrapBrotli MODULE
    PROVIDED_TARGETS WrapBrotli::WrapBrotliDec MODULE_NAME network QMAKE_LIB brotli
    VCPKG_PORT brotli
    VCPKG_ADD_TO_FEATURE brotli
)
bobui_find_package(Libproxy MODULE
    PROVIDED_TARGETS PkgConfig::Libproxy MODULE_NAME network QMAKE_LIB libproxy)
bobui_find_package(GSSAPI MODULE PROVIDED_TARGETS GSSAPI::GSSAPI MODULE_NAME network QMAKE_LIB gssapi)
bobui_find_package(GLIB2 MODULE
    OPTIONAL_COMPONENTS GOBJECT PROVIDED_TARGETS GLIB2::GOBJECT MODULE_NAME core QMAKE_LIB gobject)
bobui_find_package(GLIB2 MODULE
    OPTIONAL_COMPONENTS GIO PROVIDED_TARGETS GLIB2::GIO MODULE_NAME core QMAKE_LIB gio)
bobui_find_package(WrapResolv MODULE
    PROVIDED_TARGETS WrapResolv::WrapResolv MODULE_NAME network QMAKE_LIB libresolv)

#### Tests

# getifaddrs
bobui_config_compile_test(getifaddrs
    LABEL "getifaddrs()"
    CODE
"#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <ifaddrs.h>

int main(void)
{
    /* BEGIN TEST: */
ifaddrs *list;
getifaddrs(&list);
freeifaddrs(list);
    /* END TEST: */
    return 0;
}
"# FIXME: use: unmapped library: network
)

# ipv6ifname
bobui_config_compile_test(ipv6ifname
    LABEL "IPv6 ifname"
    CODE
"#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>

int main(void)
{
    /* BEGIN TEST: */
char buf[IFNAMSIZ];
if_nametoindex(\"eth0\");
if_indextoname(1, buf);
if_freenameindex(if_nameindex());
    /* END TEST: */
    return 0;
}
"# FIXME: use: unmapped library: network
)

# linux-netlink
bobui_config_compile_test(linux_netlink
    LABEL "Linux AF_NETLINK sockets"
    CODE
"#include <asm/types.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <sys/socket.h>

int main(void)
{
    /* BEGIN TEST: */
struct rtattr rta = { };
struct ifinfomsg ifi = {};
struct ifaddrmsg ifa = {};
struct ifa_cacheinfo ci;
ci.ifa_prefered = ci.ifa_valid = 0;
(void)RTM_NEWLINK; (void)RTM_NEWADDR;
(void)IFLA_ADDRESS; (void)IFLA_IFNAME;
(void)IFA_ADDRESS; (void)IFA_LABEL; (void)IFA_CACHEINFO;
(void)(IFA_F_SECONDARY | IFA_F_DEPRECATED | IFA_F_PERMANENT | IFA_F_MANAGETEMPADDR);
    /* END TEST: */
    return 0;
}
")

# res_setserver
bobui_config_compile_test(res_setservers
    LABEL "res_setservers()"
    LIBRARIES
        WrapResolv::WrapResolv
    CODE
"#include <sys/types.h>
#include <netinet/in.h>
#include <resolv.h>
int main()
{
    union res_sockaddr_union sa;
    res_state s = nullptr;
    res_setservers(s, &sa, 1);
    return 0;
}
"
)

# sctp
bobui_config_compile_test(sctp
    LABEL "SCTP support"
    CODE
"#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/sctp.h>

int main(void)
{
    /* BEGIN TEST: */
sctp_initmsg sctpInitMsg;
socklen_t sctpInitMsgSize = sizeof(sctpInitMsg);
(void) socket(PF_INET, SOCK_STREAM, IPPROTO_SCTP);
(void) getsockopt(-1, SOL_SCTP, SCTP_INITMSG, &sctpInitMsg, &sctpInitMsgSize);
    /* END TEST: */
    return 0;
}
"# FIXME: use: unmapped library: network
)

# dtls
bobui_config_compile_test(dtls
    LABEL "DTLS support in OpenSSL"
    LIBRARIES
        WrapOpenSSLHeaders::WrapOpenSSLHeaders
    CODE
"#include <openssl/ssl.h>
#if defined(OPENSSL_NO_DTLS) || !defined(DTLS1_2_VERSION)
#  error OpenSSL without DTLS support
#endif

int main(void)
{
    /* BEGIN TEST: */
    /* END TEST: */
    return 0;
}
")

# ocsp
bobui_config_compile_test(ocsp
    LABEL "OCSP stapling support in OpenSSL"
    LIBRARIES
        WrapOpenSSLHeaders::WrapOpenSSLHeaders
    CODE
"#include <openssl/ssl.h>
#include <openssl/ocsp.h>
#if defined(OPENSSL_NO_OCSP) || defined(OPENSSL_NO_TLSEXT)
#  error OpenSSL without OCSP stapling
#endif

int main(void)
{
    /* BEGIN TEST: */
    /* END TEST: */
    return 0;
}
")

# networklistmanager
bobui_config_compile_test(networklistmanager
    LABEL "Network List Manager"
    CODE
"#include <netlistmgr.h>
#include <ocidl.h>
#include <wrl/client.h>

int main(void)
{
    /* BEGIN TEST: */
using namespace Microsoft::WRL;
ComPtr<INetworkListManager> networkListManager;
ComPtr<IConnectionPoint> connectionPoint;
ComPtr<IConnectionPointContainer> connectionPointContainer;
networkListManager.As(&connectionPointContainer);
connectionPointContainer->FindConnectionPoint(IID_INetworkConnectionEvents, &connectionPoint);
    /* END TEST: */
    return 0;
}
"# FIXME: qmake: LIBS += -lOle32
)



#### Features

bobui_feature("getifaddrs" PUBLIC
    LABEL "getifaddrs()"
    CONDITION VXWORKS OR UNIX AND NOT BOBUI_FEATURE_linux_netlink AND TEST_getifaddrs
)
bobui_feature_definition("getifaddrs" "BOBUI_NO_GETIFADDRS" NEGATE VALUE "1")
bobui_feature("ipv6ifname" PUBLIC
    LABEL "IPv6 ifname"
    CONDITION VXWORKS OR UNIX AND NOT BOBUI_FEATURE_linux_netlink AND TEST_ipv6ifname
)
bobui_feature_definition("ipv6ifname" "BOBUI_NO_IPV6IFNAME" NEGATE VALUE "1")
bobui_feature("libresolv" PRIVATE
    LABEL "libresolv"
    CONDITION WrapResolv_FOUND
    AUTODETECT UNIX
)
bobui_feature("libproxy" PRIVATE
    LABEL "libproxy"
    AUTODETECT OFF
    CONDITION Libproxy_FOUND
)
bobui_feature("linux-netlink" PRIVATE
    LABEL "Linux AF_NETLINK"
    CONDITION LINUX AND NOT ANDROID AND TEST_linux_netlink
)
bobui_feature("res_setservers" PRIVATE
    LABEL "res_setservers()"
    CONDITION BOBUI_FEATURE_libresolv AND TEST_res_setservers
)
bobui_feature("securetransport" PUBLIC
    LABEL "SecureTransport"
    CONDITION APPLE
    DISABLE INPUT_ssl STREQUAL 'no'
)
bobui_feature_definition("securetransport" "BOBUI_SECURETRANSPORT")
bobui_feature("schannel" PUBLIC
    LABEL "Schannel"
    CONDITION WIN32
    DISABLE INPUT_ssl STREQUAL 'no'
)
bobui_feature_definition("schannel" "BOBUI_SCHANNEL")
bobui_feature("ssl" PUBLIC
    LABEL "SSL"
    CONDITION BOBUI_FEATURE_securetransport OR BOBUI_FEATURE_openssl OR BOBUI_FEATURE_schannel
)
bobui_feature_definition("ssl" "BOBUI_NO_SSL" NEGATE VALUE "1")
bobui_feature("dtls" PUBLIC
    SECTION "Networking"
    LABEL "DTLS"
    PURPOSE "Provides a DTLS implementation"
    CONDITION BOBUI_FEATURE_ssl AND BOBUI_FEATURE_openssl AND BOBUI_FEATURE_udpsocket AND TEST_dtls
)
bobui_feature("ocsp" PUBLIC
    SECTION "Networking"
    LABEL "OCSP-stapling"
    PURPOSE "Provides OCSP stapling support"
    CONDITION BOBUI_FEATURE_openssl AND TEST_ocsp
)
bobui_feature("sctp" PUBLIC
    LABEL "SCTP"
    AUTODETECT OFF
    CONDITION TEST_sctp
)
bobui_feature_definition("sctp" "BOBUI_NO_SCTP" NEGATE VALUE "1")
bobui_feature("system-proxies" PRIVATE
    LABEL "Use system proxies"
)
bobui_feature("http" PUBLIC
    SECTION "Networking"
    LABEL "HTTP"
    PURPOSE "Provides support for the Hypertext Transfer Protocol in QNetworkAccessManager."
    CONDITION BOBUI_FEATURE_thread
)
bobui_feature_definition("http" "BOBUI_NO_HTTP" NEGATE VALUE "1")
bobui_feature("udpsocket" PUBLIC
    SECTION "Networking"
    LABEL "QUdpSocket"
    PURPOSE "Provides access to UDP sockets."
)
bobui_feature_definition("udpsocket" "BOBUI_NO_UDPSOCKET" NEGATE VALUE "1")
bobui_feature("networkproxy" PUBLIC
    SECTION "Networking"
    LABEL "QNetworkProxy"
    PURPOSE "Provides network proxy support."
)
bobui_feature_definition("networkproxy" "BOBUI_NO_NETWORKPROXY" NEGATE VALUE "1")
bobui_feature("socks5" PUBLIC
    SECTION "Networking"
    LABEL "SOCKS5"
    PURPOSE "Provides SOCKS5 support in QNetworkProxy."
    CONDITION BOBUI_FEATURE_networkproxy
)
bobui_feature_definition("socks5" "BOBUI_NO_SOCKS5" NEGATE VALUE "1")
bobui_feature("networkinterface" PUBLIC
    SECTION "Networking"
    LABEL "QNetworkInterface"
    PURPOSE "Supports enumerating a host's IP addresses and network interfaces."
    CONDITION NOT WASM
)
bobui_feature_definition("networkinterface" "BOBUI_NO_NETWORKINTERFACE" NEGATE VALUE "1")
bobui_feature("networkdiskcache" PUBLIC
    SECTION "Networking"
    LABEL "QNetworkDiskCache"
    PURPOSE "Provides a disk cache for network resources."
    CONDITION BOBUI_FEATURE_temporaryfile
)
bobui_feature_definition("networkdiskcache" "BOBUI_NO_NETWORKDISKCACHE" NEGATE VALUE "1")
bobui_feature("brotli" PUBLIC
    SECTION "Networking"
    LABEL "Brotli Decompression Support"
    PURPOSE "Support for downloading and decompressing resources compressed with Brotli through QNetworkAccessManager."
    CONDITION WrapBrotli_FOUND
)
bobui_feature_definition("brotli" "BOBUI_NO_BROTLI" NEGATE VALUE "1")
bobui_feature("localserver" PUBLIC
    SECTION "Networking"
    LABEL "QLocalServer"
    PURPOSE "Provides a local socket based server."
    CONDITION BOBUI_FEATURE_temporaryfile AND NOT VXWORKS
)
bobui_feature_definition("localserver" "BOBUI_NO_LOCALSERVER" NEGATE VALUE "1")
bobui_feature("dnslookup" PUBLIC
    SECTION "Networking"
    LABEL "QDnsLookup"
    PURPOSE "Provides API for DNS lookups."
    CONDITION BOBUI_FEATURE_thread AND NOT INTEGRITY
)
bobui_feature("gssapi" PUBLIC
    SECTION "Networking"
    LABEL "GSSAPI"
    PURPOSE "Enable SPNEGO authentication through GSSAPI"
    CONDITION NOT WIN32 AND GSSAPI_FOUND
)
bobui_feature_definition("gssapi" "BOBUI_NO_GSSAPI" NEGATE VALUE "1")
bobui_feature("sspi" PUBLIC
    SECTION "Networking"
    LABEL "SSPI"
    PURPOSE "Enable NTLM/SPNEGO authentication through SSPI"
    CONDITION WIN32
)
bobui_feature_definition("sspi" "BOBUI_NO_SSPI" NEGATE VALUE "1")
bobui_feature("networklistmanager" PRIVATE
    SECTION "Networking"
    LABEL "Network List Manager"
    PURPOSE "Use Network List Manager to keep track of network connectivity"
    CONDITION WIN32 AND TEST_networklistmanager
)
bobui_feature("topleveldomain" PUBLIC
    SECTION "Networking"
    LABEL "qIsEffectiveTLD()"
    PURPOSE "Provides support for checking if a domain is a top level domain. If enabled, a binary dump of the Public Suffix List (http://www.publicsuffix.org, Mozilla License) is included. The data is used in QNetworkCookieJar."
    AUTODETECT NOT WASM
    DISABLE INPUT_publicsuffix STREQUAL "no"
)
bobui_feature("publicsuffix-bobui" PRIVATE
    LABEL "  Built-in publicsuffix database"
    CONDITION BOBUI_FEATURE_topleveldomain
    ENABLE INPUT_publicsuffix STREQUAL "bobui" OR INPUT_publicsuffix STREQUAL "all"
    DISABLE INPUT_publicsuffix STREQUAL "system"
)
bobui_feature("publicsuffix-system" PRIVATE
    LABEL "  System publicsuffix database"
    CONDITION BOBUI_FEATURE_topleveldomain
    AUTODETECT LINUX OR HURD
    ENABLE INPUT_publicsuffix STREQUAL "system" OR INPUT_publicsuffix STREQUAL "all"
    DISABLE INPUT_publicsuffix STREQUAL "bobui"
)

bobui_configure_add_summary_section(NAME "BobUI Network")
bobui_configure_add_summary_entry(ARGS "getifaddrs")
bobui_configure_add_summary_entry(ARGS "ipv6ifname")
bobui_configure_add_summary_entry(ARGS "libproxy")
bobui_configure_add_summary_entry(
    ARGS "linux-netlink"
    CONDITION LINUX
)
bobui_configure_add_summary_entry(
    ARGS "securetransport"
    CONDITION APPLE
)
bobui_configure_add_summary_entry(
    ARGS "schannel"
    CONDITION WIN32
)
bobui_configure_add_summary_entry(ARGS "dtls")
bobui_configure_add_summary_entry(ARGS "ocsp")
bobui_configure_add_summary_entry(ARGS "sctp")
bobui_configure_add_summary_entry(ARGS "system-proxies")
bobui_configure_add_summary_entry(ARGS "gssapi")
bobui_configure_add_summary_entry(ARGS "brotli")
bobui_configure_add_summary_entry(ARGS "topleveldomain")
bobui_configure_add_summary_entry(ARGS "publicsuffix-bobui")
bobui_configure_add_summary_entry(ARGS "publicsuffix-system")
bobui_configure_end_summary_section() # end of "BobUI Network" section
