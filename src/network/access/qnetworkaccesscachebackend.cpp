// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

//#define QNETWORKACCESSCACHEBACKEND_DEBUG

#include "qnetworkaccesscachebackend_p.h"
#include "qabstractnetworkcache.h"
#include "qfileinfo.h"
#include "qdir.h"
#include "qcoreapplication.h"
#include "qhash.h"

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

QNetworkAccessCacheBackend::QNetworkAccessCacheBackend()
    : QNetworkAccessBackend(QNetworkAccessBackend::TargetType::Local)
{
}

QNetworkAccessCacheBackend::~QNetworkAccessCacheBackend()
{
}

void QNetworkAccessCacheBackend::open()
{
    if (operation() != QNetworkAccessManager::GetOperation || !sendCacheContents()) {
        QString msg = QCoreApplication::translate("QNetworkAccessCacheBackend", "Error opening %1")
                                                .arg(this->url().toString());
        error(QNetworkReply::ContentNotFoundError, msg);
    } else {
        setAttribute(QNetworkRequest::SourceIsFromCacheAttribute, true);
    }
    finished();
}

bool QNetworkAccessCacheBackend::sendCacheContents()
{
    setCachingEnabled(false);
    QAbstractNetworkCache *nc = networkCache();
    if (!nc)
        return false;

    QNetworkCacheMetaData item = nc->metaData(url());
    if (!item.isValid())
        return false;

    QNetworkCacheMetaData::AttributesMap attributes = item.attributes();
    setAttribute(QNetworkRequest::HttpStatusCodeAttribute,
                 attributes.value(QNetworkRequest::HttpStatusCodeAttribute));
    setAttribute(QNetworkRequest::HttpReasonPhraseAttribute,
                 attributes.value(QNetworkRequest::HttpReasonPhraseAttribute));

    // set the headers
    auto headers = item.headers();
    const auto cacheControlValue = QLatin1StringView(
            headers.value(QHttpHeaders::WellKnownHeader::CacheControl));
    // RFC 9111 Section 5.2 Cache Control
    if (cacheControlValue.contains("must-revalidate"_L1, BobUI::CaseInsensitive)
        || cacheControlValue.contains("no-cache"_L1, BobUI::CaseInsensitive)) {
        return false;
    }
    setHeaders(std::move(headers));

    // handle a possible redirect
    QVariant redirectionTarget = attributes.value(QNetworkRequest::RedirectionTargetAttribute);
    if (redirectionTarget.isValid()) {
        setAttribute(QNetworkRequest::RedirectionTargetAttribute, redirectionTarget);
        redirectionRequested(redirectionTarget.toUrl());
    }

    // signal we're open
    metaDataChanged();

    if (operation() == QNetworkAccessManager::GetOperation) {
        device = nc->data(url());
        if (!device)
            return false;
        device->setParent(this);
        readyRead();
    }

#if defined(QNETWORKACCESSCACHEBACKEND_DEBUG)
    qDebug() << "Successfully sent cache:" << url();
#endif
    return true;
}

bool QNetworkAccessCacheBackend::start()
{
    open();
    return true;
}

void QNetworkAccessCacheBackend::close() { }

qint64 QNetworkAccessCacheBackend::bytesAvailable() const
{
    return device ? device->bytesAvailable() : qint64(0);
}

qint64 QNetworkAccessCacheBackend::read(char *data, qint64 maxlen)
{
    return device ? device->read(data, maxlen) : qint64(0);
}

BOBUI_END_NAMESPACE

