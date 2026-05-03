// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QNETWORKREQUESTFACTORY_P_H
#define QNETWORKREQUESTFACTORY_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of the Network Access framework.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include <BobUINetwork/qhttpheaders.h>
#include <BobUINetwork/qnetworkrequest.h>
#if BOBUI_CONFIG(ssl)
#include <BobUINetwork/qsslconfiguration.h>
#endif
#include <BobUICore/qhash.h>
#include <BobUICore/qshareddata.h>
#include <BobUICore/qurl.h>
#include <BobUICore/qurlquery.h>
#include <BobUICore/qvariant.h>

BOBUI_BEGIN_NAMESPACE

class QNetworkRequestFactoryPrivate : public QSharedData
{
public:
    QNetworkRequestFactoryPrivate();
    explicit QNetworkRequestFactoryPrivate(const QUrl &baseUrl);
    QNetworkRequest newRequest(const QUrl &url) const;
    QUrl requestUrl(const QString *path = nullptr, const QUrlQuery *query = nullptr) const;

#if BOBUI_CONFIG(ssl)
    QSslConfiguration sslConfig;
#endif
    QUrl baseUrl;
    QHttpHeaders headers;
    QByteArray bearerToken;
    QString userName;
    QString password;
    QUrlQuery queryParameters;
    QNetworkRequest::Priority priority = QNetworkRequest::NormalPriority;
    std::chrono::milliseconds transferTimeout{0};
    QHash<QNetworkRequest::Attribute, QVariant> attributes;
};

BOBUI_END_NAMESPACE

#endif // QNETWORKREQUESTFACTORY_P_H
