// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qnetworkproxy.h"

#include <BobUICore/qapplicationstatic.h>
#include <BobUICore/qcoreapplication_platform.h>
#include <BobUICore/qjnienvironment.h>
#include <BobUICore/qjniobject.h>

#ifndef BOBUI_NO_NETWORKPROXY

BOBUI_BEGIN_NAMESPACE

struct ProxyInfoObject
{
public:
    ProxyInfoObject();
    ~ProxyInfoObject();
};

using namespace QNativeInterface;
using namespace BobUIJniTypes;

Q_APPLICATION_STATIC(ProxyInfoObject, proxyInfoInstance)

Q_DECLARE_JNI_CLASS(BobUINetwork, "org/bobuiproject/bobui/android/network/BobUINetwork")
Q_DECLARE_JNI_CLASS(ProxyInfo, "android/net/ProxyInfo")

ProxyInfoObject::ProxyInfoObject()
{
    BobUINetwork::callStaticMethod<void>("registerReceiver", QAndroidApplication::context());
}

ProxyInfoObject::~ProxyInfoObject()
{
    BobUINetwork::callStaticMethod<void>("unregisterReceiver", QAndroidApplication::context());
}

QList<QNetworkProxy> QNetworkProxyFactory::systemProxyForQuery(const QNetworkProxyQuery &query)
{
    QList<QNetworkProxy> proxyList;
    if (!proxyInfoInstance)
        return proxyList;

    QJniObject proxyInfo = BobUINetwork::callStaticMethod<ProxyInfo>("getProxyInfo",
                                                                  QAndroidApplication::context());
    if (proxyInfo.isValid()) {
        const QJniArray exclusionList = proxyInfo.callMethod<String[]>("getExclusionList");
        bool exclude = false;
        if (exclusionList.isValid()) {
            const QUrl host = QUrl(query.url().host());
            for (const auto &entry : exclusionList) {
                if (host.matches(QUrl(entry.toString()), QUrl::RemoveScheme)) {
                    exclude = true;
                    break;
                }
            }
        }
        if (!exclude) {
            const QString hostName = proxyInfo.callMethod<QString>("getHost");
            const int port = proxyInfo.callMethod<jint>("getPort");
            QNetworkProxy proxy(QNetworkProxy::HttpProxy, hostName, port);
            proxyList << proxy;
        }
    }
    if (proxyList.isEmpty())
        proxyList << QNetworkProxy::NoProxy;

    return proxyList;
}

BOBUI_END_NAMESPACE

#endif
