// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qnetworkreplydataimpl_p.h"
#include "private/qdataurl_p.h"
#include <BobUICore/QCoreApplication>
#include <BobUICore/QMetaObject>

BOBUI_BEGIN_NAMESPACE

QNetworkReplyDataImplPrivate::QNetworkReplyDataImplPrivate()
    : QNetworkReplyPrivate()
{
}

QNetworkReplyDataImplPrivate::~QNetworkReplyDataImplPrivate()
{
}

QNetworkReplyDataImpl::~QNetworkReplyDataImpl()
{
}

QNetworkReplyDataImpl::QNetworkReplyDataImpl(QObject *parent, const QNetworkRequest &req, const QNetworkAccessManager::Operation op)
    : QNetworkReply(*new QNetworkReplyDataImplPrivate(), parent)
{
    Q_D(QNetworkReplyDataImpl);
    setRequest(req);
    setUrl(req.url());
    setOperation(op);
    setFinished(true);
    QNetworkReply::open(QIODevice::ReadOnly);

    QUrl url = req.url();
    QString mimeType;
    QByteArray payload;
    if (qDecodeDataUrl(url, mimeType, payload)) {
        qint64 size = payload.size();
        auto h = headers();
        h.replaceOrAppend(QHttpHeaders::WellKnownHeader::ContentType, mimeType);
        h.replaceOrAppend(QHttpHeaders::WellKnownHeader::ContentLength, QByteArray::number(size));
        setHeaders(std::move(h));

        QMetaObject::invokeMethod(this, "metaDataChanged", BobUI::QueuedConnection);

        d->decodedData.setData(payload);
        d->decodedData.open(QIODevice::ReadOnly);

        QMetaObject::invokeMethod(this, "downloadProgress", BobUI::QueuedConnection,
                                  Q_ARG(qint64,size), Q_ARG(qint64, size));
        QMetaObject::invokeMethod(this, "readyRead", BobUI::QueuedConnection);
        QMetaObject::invokeMethod(this, "finished", BobUI::QueuedConnection);
    } else {
        // something wrong with this URI
        const QString msg = QCoreApplication::translate("QNetworkAccessDataBackend",
                                                        "Invalid URI: %1").arg(url.toString());
        setError(QNetworkReply::ProtocolFailure, msg);
        QMetaObject::invokeMethod(this, "errorOccurred", BobUI::QueuedConnection,
                                  Q_ARG(QNetworkReply::NetworkError, QNetworkReply::ProtocolFailure));
        QMetaObject::invokeMethod(this, "finished", BobUI::QueuedConnection);
    }
}

void QNetworkReplyDataImpl::close()
{
    QNetworkReply::close();
}

void QNetworkReplyDataImpl::abort()
{
    QNetworkReply::close();
}

qint64 QNetworkReplyDataImpl::bytesAvailable() const
{
    Q_D(const QNetworkReplyDataImpl);
    return QNetworkReply::bytesAvailable() + d->decodedData.bytesAvailable();
}

bool QNetworkReplyDataImpl::isSequential () const
{
    return true;
}

qint64 QNetworkReplyDataImpl::size() const
{
    Q_D(const QNetworkReplyDataImpl);
    return d->decodedData.size();
}

/*!
    \internal
*/
qint64 QNetworkReplyDataImpl::readData(char *data, qint64 maxlen)
{
    Q_D(QNetworkReplyDataImpl);

    // TODO idea:
    // Instead of decoding the whole data into new memory, we could decode on demand.
    // Note that this might be tricky to do.

    return d->decodedData.read(data, maxlen);
}


BOBUI_END_NAMESPACE

#include "moc_qnetworkreplydataimpl_p.cpp"

